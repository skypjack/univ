/**
 * Worker is the server component involved into the proposed protocol. It
 * cooperates with the client part exploiting available nets to obtain results.
 *
 * @author Michele Caini
 */

package nnsec.server;

import java.util.Timer;
import java.util.TimerTask;
import java.util.Iterator;
import java.math.BigInteger;
import java.rmi.RemoteException;
import java.rmi.server.UnicastRemoteObject;
import java.rmi.server.Unreferenced;
import nnsec.net.NetManager;
import nnsec.net.NetIFace;
import nnsec.cryptosystem.Cryptosystem;
import nnsec.cryptosystem.Mapper;
import nnsec.client.Calculator_Stub;

/**
 * This class implements the related interface to turn himself into a remote
 * class, so that clients can ask for a remote instance of Worker.
 */
public class Worker implements WorkerIFace, Unreferenced {

	/** Reference to the local NetManager. */
	private NetManager nm;

	/** Client public ket (to be setted up by the client). */
	private Cryptosystem.PublicKey puk;

	/** Cryptosystem s-parameter value (to be setted up by the client). */
	private int svalue;

	/** A mapper used during protocol exploitation. */
	private Mapper mapper;

	/** Neural network reference, which is the net Worker works with. */
	private NetIFace net;

	/** Neural network identifier (useful to work with NetManager). */
	private int netID;

	/** Total time spent by this unit until now. */
	private long gap;

	/**
	 * Exception thrown when trying to exploit the protocol, and then some
	 * errors occur.
	 */
	static public class ProtocolException extends RemoteException {

		/**
		 * Constructs a ProtocolException.
		 */
		public ProtocolException() { super(); }

		/**
		 * Constructs a ProtocolException with the specified detail
		 * message.
		 * @param msg the detail message.
		 */
		public ProtocolException(String msg) { super (msg); }
	}

	/**
	 * Constructs a Worker with a NetManager reference, which is the
	 * NetManager Worker works with.
	 * @param nmref NetManager reference.
	 * @throws RemoteException default exception, as indicated in the rmi
	 * 	reference manual.
	 */
	public Worker(NetManager nmref) throws RemoteException {
		nm = nmref;
		puk = null;
		svalue = 1;
		mapper = null;
		net = null;
		netID = 0;
		gap = 0;
	}

	/**
	 * This method permits to free a locked neural network (if exists one)
	 * if the client crashes and no reference exists to the Worker.
	 */
	public void unreferenced() {
		try { unlockNet(); }
		catch (RemoteException re) {}
	}

	/**
	 * Method used to lock a neural network by identifier.
	 * @param id neural network identifier.
	 * @return true if the network which identifier refers to exists, false
	 * 	otherwise.
	 * @throws RemoteException default exception, as indicated in the rmi
	 * 	reference manual.
	 */
	public boolean lockNet(int id) throws RemoteException {
		netID = id;
		net = nm.lockNet(id);
		if(net != null)
			return true;
		return false;
	}

	/**
	 * Method used to unlock a locked neural network (if there is one).
	 * @throws RemoteException default exception, as indicated in the rmi
	 * 	reference manual.
	 */
	public void unlockNet() throws RemoteException {
		if(net != null) {
			nm.unlockNet(netID);
			net = null;
		}
	}

	/**
	 * This method permits to get an upper bound of the weights of the
	 * locked neural network (if there is one) as norm.
	 * @return neural network weights upper bound.
	 * @throws RemoteException default exception, as indicated in the rmi
	 * 	reference manual.
	 */
	public int getWeightsUpperBound() throws RemoteException {
		double ret;
		ret = 0;
		if(net != null) {
			Iterator<NetIFace.Node> nit = net.getHiddenLayerIterator();
			while(nit.hasNext()) {
				NetIFace.Node hnode = nit.next();
				Iterator<NetIFace.Link> hlit = hnode.getInputLinksIterator();
				while(hlit.hasNext())
					ret += Math.pow(hlit.next().getWeight(), 2);
			}
			nit = net.getOutputLayerIterator();
			while(nit.hasNext()) {
				NetIFace.Node onode = nit.next();
				Iterator<NetIFace.Link> olit = onode.getInputLinksIterator();
				while(olit.hasNext())
					ret += Math.pow(olit.next().getWeight(), 2);
			}

		}
		ret = Math.sqrt(ret);
		return (int) Math.ceil(ret);
	}

	/**
	 * Method that permits to know which is the scale associated with a
	 * locked neural network (if there is one).
	 * @return neural network scale.
	 * @throws RemoteException default exception, as indicated in the rmi
	 * 	reference manual.
	 */
	public int getNetScale() throws RemoteException {
		int sc = 0;
		if(net != null)
			sc = nm.getNetScale(netID);
		else sc = 0;
		return sc;
	}

	/**
	 * Method used by the clients to set the cryptosystem environment
	 * (public key and s-parameter value).
	 * @param puk    client's public key.
	 * @param svalue s-parameter value.
	 * @throws RemoteException default exception, as indicated in the rmi
	 * 	reference manual.
	 */
	public void setCryptoEnvironment(String puk, int svalue) throws RemoteException {
		this.puk = new Cryptosystem.PublicKey(new BigInteger(puk));
		this.svalue = svalue;
	}

	/**
	 * This method permits to compute the value of the node, based on the
	 * weights of the input links and the value of the connected nodes.
	 * Lastly, the value of the node is computed using a specific activation
	 * function and, if exists, an output function. If an output function
	 * doesn't exist for the node and the net has not a default output
	 * function, identity function will be theoretically used (the value
	 * will be not changed again).
	 * Cliet reference is necessary to compute the value, because only the
	 * client can decrypts the value, computes results using activation
	 * function, then encrypts this new values and turn back to the server.
	 * @param node node reference.
	 * @param calc calculator remote reference.
	 * @throws ProtocolException thrown as a consequence of errors during
	 * 	computation of the node value.
	 */
	private void computeNodeValue(NetIFace.Node node, Calculator_Stub calc) throws ProtocolException {
		long hook = System.currentTimeMillis();
		BigInteger value = BigInteger.ONE;
		Iterator<NetIFace.Link> lit = node.getInputLinksIterator();
		while(lit.hasNext()) {
			NetIFace.Link link = lit.next();
			value = Cryptosystem.combineCiphertexts(puk, value,
					Cryptosystem.raiseCiphertext(puk, link.getOutputNode().getValue(),
					mapper.realToPositiveInteger(link.getWeight()), svalue), svalue);
		}
		if(node.hasThreshold()) {
			BigInteger one = null;
			try { one = puk.encrypt(mapper.realToPositiveInteger(1), svalue); }
			catch (Exception excp) { throw new ProtocolException(excp.getMessage()); }
			value = Cryptosystem.combineCiphertexts(puk, value,
					Cryptosystem.raiseCiphertext(puk, one,
					mapper.realToPositiveInteger(node.getThreshold()), svalue), svalue);
		}
		try {
			boolean swap = false;
			String function;
			if(node.hasAssociatedActvFunction())
				function = node.getAssociatedActvFunction();
			else if(net.hasDefaultActvFunction())
				function = net.getDefaultActvFunction();
			else function = null;
			if(function != null) {
			        if(Math.random() > Math.random()) {
					swap = true;
					value = Cryptosystem.raiseCiphertext(puk, value,
							mapper.mapTo(new BigInteger("-1")),
							svalue);
				}
				gap += System.currentTimeMillis() - hook;
				value = new BigInteger(calc.computeValue(value.toString(), function));
				hook = System.currentTimeMillis();
				if(swap) {
					swap = false;
					value = Cryptosystem.combineCiphertexts(puk,
							puk.encrypt(mapper.realToPositiveInteger(1), svalue),
							Cryptosystem.raiseCiphertext(puk, value,
								mapper.mapTo(new BigInteger("-1")),
								svalue), svalue);
				}
			}
			if(node.hasAssociatedOutFunction())
				function = node.getAssociatedOutFunction();
			else if(net.hasDefaultOutFunction())
				function = net.getDefaultOutFunction();
			else function = null;
			if(function != null) {
				if(Math.random() > Math.random()) {
					swap = true;
					value = Cryptosystem.raiseCiphertext(puk, value,
						        mapper.mapTo(new BigInteger("-1")),
							svalue);
				}
				gap += System.currentTimeMillis() - hook;
				value = new BigInteger(calc.mangleValue(value.toString(), function));
				hook = System.currentTimeMillis();
				if(swap) {
					swap = false;
					value = Cryptosystem.combineCiphertexts(puk,
							puk.encrypt(mapper.realToPositiveInteger(1), svalue),
							Cryptosystem.raiseCiphertext(puk, value,
								mapper.mapTo(new BigInteger("-1")),
								svalue), svalue);
				}
			}
		} catch (Exception excp) {
			throw new ProtocolException(excp.getMessage());
		}
		node.setValue(value);
		gap += System.currentTimeMillis() - hook;
	}

	/**
	 * Method used by the client to get outputs from a neural network.
	 * @param inputs input values of the neural network.
	 * @param calc   calculator remote reference, used to compute nodes
	 * 	values.
	 * @return output values of the neural network.
	 * @throws ProtocolException thrown as a consequence of errors during
	 * 	exploitation of the protocol.
	 */
	public String[] getOutputs(String[] inputs, Calculator_Stub calc) throws ProtocolException {
		long pre = System.currentTimeMillis();
		if((nm == null) || (puk == null) || (net == null))
			throw new ProtocolException("uninitialized enviroment ...");
		try { mapper = new Mapper(getNetScale(), puk.getModulus().pow(svalue)); }
		catch (Exception excp) { throw new ProtocolException(excp.getMessage()); }
		if(inputs.length != net.getInputLayerSize())
			throw new ProtocolException("no more inputs ...");
		Iterator<NetIFace.Node> nit = net.getInputLayerIterator();
		int iter = 0;
		while(nit.hasNext())
			nit.next().setValue(new BigInteger(inputs[iter++]));
		nit = net.getHiddenLayerIterator();
		long post = System.currentTimeMillis();
		gap = post - pre;
		while(nit.hasNext())
			computeNodeValue(nit.next(), calc);
		nit = net.getOutputLayerIterator();
		while(nit.hasNext())
			computeNodeValue(nit.next(), calc);
		pre = System.currentTimeMillis();
		String[] outputs = new String[net.getOutputLayerSize()];
		nit = net.getOutputLayerIterator();
		iter = 0;
		while(nit.hasNext())
			outputs[iter++] = nit.next().getValue().toString();
		post = System.currentTimeMillis();
		gap += post - pre;
		return outputs;
	}

	/**
	 * This method permits to get a detailed list of available neural
	 * networks (nets exported by the server).
	 * @return detailed list of available neural networks (using a specific
	 * 	syntax).
	 * @throws RemoteException default exception, as indicated in the rmi
	 * 	reference manual.
	 */
	public Object[][] getNetsList() throws RemoteException {
		return nm.getAvailableNetsList();
	}

	/**
	 * This method permits to know the time spent by a Worker during
	 * last transiction, that is, how much time a Worker really works.
	 * @return on-board Worker gap.
	 * @throws RemoteException default exception, as indicated in the rmi
	 * 	reference manual.
	 */
	public long getWorkerGap() throws RemoteException {
		return gap;
	}
}
