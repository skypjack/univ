/**
 * CommModule is the client component involved into the proposed protocol. It
 * cooperates with the server part exploiting available nets to obtain results.
 *
 * @author Michele Caini
 */

package nnsec.client;

import java.math.BigInteger;
import java.math.BigDecimal;
import java.rmi.AccessException;
import java.rmi.registry.Registry;
import java.rmi.registry.LocateRegistry;
import java.rmi.RemoteException;
import java.rmi.server.UnicastRemoteObject;
import javax.rmi.ssl.SslRMIClientSocketFactory;
import nnsec.server.WorkerFactory_Stub;
import nnsec.server.Worker_Stub;
import nnsec.cryptosystem.Cryptosystem;
import nnsec.cryptosystem.Mapper;

/**
 * This class implements the related interface to turn himself into a remote
 * class, so that users can export remote instances of CommModule.
 */
public class CommModule {

	/** Associated remote server part. */
	private Worker_Stub wif;

	/** Protocol related scale. */
	private int scale;

	/** Total execution gap **/
	private long totalGap;

	/** Local execution gap **/
	private long localGap;

	/**
	 * Exception thrown when trying to connect to the server, and then some
	 * errors occur.
	 */
	static public class ConnectionException extends Exception {

		/**
		 * Constructs a ConnectionException.
		 */
		public ConnectionException() { super(); }

		/**
		 * Constructs a ConnectionException with the specified detail
		 * message.
		 * @param msg the detail message.
		 */
		public ConnectionException(String msg) { super(msg); }
	}

	/**
	 * Default CommModule constructor, it sets associated server part to
	 * null.
	 * @throws RemoteException default exception, as indicated in the rmi
	 * 	reference manual.
	 */
	public CommModule() throws RemoteException {
		wif = null;
		totalGap = 0;
		localGap = 0;
	}

	/**
	 * This method permits to create connection with the server, then the client
	 * can retrieves an associated server part with which it can exploits
	 * the protocol.
	 * @param host       server ip address.
	 * @param port       server port.
	 * @param identifier server identifier (name used to bind the object in
	 * 	the rmi	registry).
	 * @throws ConnectionException thrown when some errors occur connecting
	 * 	with the server.
	 */
	public void createConnection(String host, int port, String identifier) throws ConnectionException {
		destroyConnection();
		try {
			Registry registry = LocateRegistry.getRegistry(host, port,
					new SslRMIClientSocketFactory());
			WorkerFactory_Stub stub = (WorkerFactory_Stub) registry.lookup(identifier);
			wif = (Worker_Stub) stub.getWorker();
			if(wif == null)
				throw new ConnectionException("Server is not able to handle incoming calls.");
		} catch (Exception excp) {
			throw new ConnectionException(excp.getMessage());
		}
	}

	/**
	 * This method destroies previously estabilished connection. In fact, it
	 * simply sets to null the server part remote reference.
	 */
	public void destroyConnection() {
		wif = null;
	}

	/**
	 * This method permits to control if a connection is available. In fact,
	 * it simply check if there is a valid server part remote reference.
	 * @return true if a connection is available, false otherwise.
	 */
	public boolean isConnectionAvailable() {
		return (wif != null);
	}

	/**
	 * Method that permits to the users to get a list of available nets with
	 * related informations.
	 * @return Array of Object array that contains information about
	 * 	available nets.
	 * @throws ConnectionException thrown as a consequence of
	 * 	RemoteException exception.
	 */
	public Object[][] getNetsList() throws ConnectionException {
		if(wif != null) {
			try { 
				Object[][] nl = wif.getNetsList();
				return nl;
			}
			catch (RemoteException re) {
				throw new ConnectionException(re.getMessage());
			}
		} else return null;
	}

	/**
	 * This method permits to use a remote neural network showed by the
	 * server, submitting inputs and obtaining results. This is the client
	 * central method involved into the protocol.
	 * @param netID  neural network identifier.
	 * @param inputs neural network inputs, in a specific syntax.
	 * @return neural network outputs, in a specific syntax.
	 * @throws ConnectionException thrown as a consequence of
	 * 	RemoteException excesption.
	 */
	public String[] getOutputs(int netID, String[] inputs) throws ConnectionException {
		totalGap = 0;
		localGap = 0;
		if(wif != null) {
			try {
				Cryptosystem cs = Cryptosystem.getInstance();
				boolean stat = wif.lockNet(netID);
				if(stat == false)
					throw new ConnectionException("unable to retrieve net " + netID + " ...");
				scale = wif.getNetScale();
				int wub = wif.getWeightsUpperBound();
				double isum = 0;
				for(int iter = 0; iter < inputs.length; iter++)
					isum += Math.pow(Double.parseDouble(inputs[iter]), 2);
				isum = Math.sqrt(isum);
				int iub = (int) Math.ceil(isum);
				BigInteger logarg = new BigInteger(new String("" + (int) Math.ceil(2 * iub * wub * Math.pow(10, 2 * scale))));
				BigInteger modulus = cs.getPublicKey().getModulus();
				int s = 0;
				while(modulus.pow(++s).compareTo(logarg) == -1);
				cs.setSParameter(s);
				wif.setCryptoEnvironment(cs.getPublicKey().getModulus().toString(), s);
				String[] cinputs = new String[inputs.length];
				Mapper mapper = new Mapper(scale, cs.getPublicKey().getModulus().pow(s));
				for(int iter = 0; iter < inputs.length; iter++)
					cinputs[iter] = cs.encrypt(mapper.realToPositiveInteger(Double.parseDouble(inputs[iter]))).toString();
				Calculator calc = new Calculator(scale);
				CalculatorIFace cstub = (CalculatorIFace) UnicastRemoteObject.exportObject(calc, 0);
				long pre = System.currentTimeMillis();
				String[] coutputs = wif.getOutputs(cinputs, (Calculator_Stub) cstub);
				long post = System.currentTimeMillis();
				while(!UnicastRemoteObject.unexportObject(calc, true));
				String[] outputs = new String[coutputs.length];
				for(int iter = 0; iter < outputs.length; iter++)
					outputs[iter] = mapper.positiveIntegerToReal(cs.decrypt(new BigInteger(coutputs[iter]))).toString();
				totalGap += post - pre;
				localGap = calc.getOnBoardGap();
				return outputs;
			} catch (Cryptosystem.MessageTooBigException mtbe) {
				throw new ConnectionException(mtbe.getMessage());
			} catch (RemoteException re) {
				throw new ConnectionException(re.getMessage());
			} finally { try { wif.unlockNet(); } catch(RemoteException re) {}  }
		} else throw new ConnectionException("connection is not available ...");
	}

	/**
	 * Returns the size of total gap, that is, total time spent to get
	 * outputs during last request.
	 * @return the total gap
	 */
	public long getTotalGap() {
		return totalGap;
	}

	/**
	 * Returns the size of local gap, that is, local time spent on this
	 * unit to get outputs during last request.
	 * return tha local gap
	 */
	public long getLocalGap() {
		return localGap;
	}

	/**
	 * Returns the size of worker gap, that is, time spent by the worker
	 * to compute outputs during last request.
	 * return tha worker gap
	 */
	public long getWorkerGap() {
		try {
			return wif.getWorkerGap();
		} catch (RemoteException re) {
			return 0;
		}
	}
}
