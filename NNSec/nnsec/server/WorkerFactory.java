/**
 * WorkerFactory is the server component that returns Worker objects to the
 * client. Worker objects are the server part involved into the protocol and
 * every client should has his personal Worker.
 */

package nnsec.server;

import java.rmi.registry.Registry;
import java.rmi.registry.LocateRegistry;
import java.rmi.Remote;
import java.rmi.RemoteException;
import java.rmi.server.UnicastRemoteObject;
import java.rmi.RMISecurityManager;
import javax.rmi.ssl.SslRMIClientSocketFactory;
import javax.rmi.ssl.SslRMIServerSocketFactory;
import nnsec.net.NetManager;

/**
 * WorkerFactory constructs new Worker objects, exports them and then returns an
 * interface to this objects so that client can communicates with them.
 */
public class WorkerFactory implements WorkerFactoryIFace {

	/** This flag indicates if the server must accepts incoming calls. */
	private boolean accept;

	/** Check about registry existence. */
	private Registry registry;;

	/** NetManager reference, useful to initialize Worker objects. */
	private NetManager nm;

	/** WorkerFactory binding identifier. */
	private String id;

	/**
	 * Default constructor for the WorkerFactory.
	 * @param nm NetManager reference.
	 * @throws RemoteException default exception, as indicated in the rmi
	 * 	reference manual.
	 */
	public WorkerFactory(NetManager nm) throws RemoteException {
		this.nm = nm;
		id = null;
		registry = null;
		accept = false;
	}

	/**
	 * Exception thrown when trying to start to the server, and then some
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
	 * This method permits to create a new remote Worker reference (if the
	 * server is active).
	 * @return remote Worker reference.
	 * @throws RemoteException defaul exception, as indicated in the rmi
	 * 	reference manual.
	 */
	public WorkerIFace getWorker() throws RemoteException {
		if(accept) {
			WorkerIFace wif = new Worker(nm);
			WorkerIFace wifstub = (WorkerIFace) UnicastRemoteObject.exportObject(wif/*, 0*/);
			return wifstub;
		} else return null;
	}

	/**
	 * This method permits to control if a connection is available.
	 * @return true if a connection is available, false otherwise.
	 */
	public boolean isConnectionAvailable() {
		return (registry != null);
	}

	/**
	 * Method that indicates if server accepts incoming calls.
	 * @return true if server accepts incoming calls, false otherwise.
	 */
	public boolean isActive() {
		return accept;
	}

	/**
	 * This method sets the ability of the server to accept incoming calls.
	 * @param accept true if the server will has to accept incoming calls.
	 * 	false otherwise.
	 */
	public void setActive(boolean accept) {
		this.accept = accept;
	}

	/**
	 * This method permits to create connection with the server, then the client
	 * can retrieves an associated server part with which it can exploits
	 * the protocol.
	 * @param port       registry server port.
	 * @param identifier server identifier (name used to bind the object in
	 * 	the rmi	registry).
	 * @throws ConnectionException thrown when some errors occur connecting
	 * 	with the server.
	 */
	public void createConnection(int port, String identifier) throws ConnectionException {
		if(registry == null) try {
			id = identifier;
			if(System.getSecurityManager() == null)
				System.setSecurityManager(new RMISecurityManager());
			registry = LocateRegistry.createRegistry(port,
				new SslRMIClientSocketFactory(),
				new SslRMIServerSocketFactory());
			WorkerFactoryIFace stub = (WorkerFactoryIFace) UnicastRemoteObject.exportObject(this, 0,
					new SslRMIClientSocketFactory(),
					new SslRMIServerSocketFactory());
			registry.bind(id, stub);
			accept = true;
		} catch (Exception excp) {
			registry = null;
			throw new ConnectionException(excp.getMessage());
		}
	}
}
