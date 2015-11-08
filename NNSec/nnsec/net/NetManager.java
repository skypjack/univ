/**
 * Instances of this class can handles objects that implements NetIFace
 * inteface. In fact, it can loads/reloads/unloads nets from file and it is
 * thought up to control access to this objects.
 *
 * @author Michele Caini
 */

package nnsec.net;

import java.util.HashMap;
import java.util.Iterator;
import java.io.File;
import java.io.FileReader;
import java.io.Serializable;
import java.util.concurrent.Semaphore;
import java.util.Vector;
import java.lang.Math;
import nnsec.parser.Parser;

/**
 * NetManager class manages objects that implements NetIface interface and
 * controls access to this objects.
 */
public class NetManager {
	
	/** Unique id to be assigned to NetIFace instances. */
	private static int netIDCounter;
	
	/** Managed nets pool. */
	private HashMap<Integer, NetHandler> netList;

	/**
	 * Exception thrown when trying to load a net from file, and then some
	 * error occurs. 
	 */
	static public class NetLoadingException extends Exception {

		/**
		 * Constructs a NetLoadingException.
		 */
		public NetLoadingException() { super(); }

		/**
		 * Constructs a NetLoadingException with the specified detail
		 * message.
		 * @param msg the detail message.
		 */
		public NetLoadingException(String msg) { super (msg); }
	}

	/**
	 * NetHandler manages a single instance of net object, controls access
	 * to the net and stores extra net related informations.
	 */
	private class NetHandler {

		/** Net reference. */
		private NetWrapper net;

		/** Semaphore, useful to control access to the net. */
		private Semaphore semaphore;

		/** Fake nodes to be added to the hidden layer of the net. */
		private int ext;

		/** Scale value, useful to avoid loss of information during
		 * encryption/decryption. This parameter permits also to use
		 * float values as inputs, not only integer values. */
		private int scale;

		/** Availability of the net. */
		private boolean available;

		/** Net filename on local filesystem. */
		private String filename;

		/**
		 * NetHandler public constructor.
		 * @param net      net reference.
		 * @param filename net filename on local filesystem.
		 */
		public NetHandler(NetIFace net, String filename) {
			this.net = new NetWrapper(net, 0);
			this.filename = filename;
			semaphore = new Semaphore(1);
			ext = 0;
			scale = 0;
			available = false;
		}

		/**
		 * This method returns some useful net informations.
		 * @return net related informations.
		 */
		public Object[] retrieveLimitedNetInfo() {
			return new Object[] { new String(net.getName()),
				new String(net.getDescription()),
				new Integer(net.getInputLayerSize()),
				new Integer(net.getOutputLayerSize()) };
		}

		/**
		 * This method returns some useful net informations.
		 * @return net related informations.
		 */
		public Object[] retrieveFullNetInfo() {
			return new Object[] { new String(net.getName()),
				new String(net.getDescription()),
				new Integer(net.getInputLayerSize()),
				new Integer(net.getOutputLayerSize()),
				new Integer(net.getHiddenLayerSize() - ext),
				new String(filename), new Integer(ext),
				new Integer(scale), new Boolean(available) };
		}


		/**
		 * Method that returns the net filename on local filesystem.
		 * @return net filename on local filesystem.
		 */
		public String getFilename() {
			return filename;
		}

		/**
		 * Method that controls access to the net and returns net
		 * reference only if the net isn't in use.
		 * @return net reference.
		 */
		public NetIFace lockNet() {
			try { semaphore.acquire(); }
			catch (InterruptedException ie) { return null; }
			net.scrambleGroups();
			return net;
		}

		/**
		 * Method that unlocks net reference, previously locked using
		 * lockNet() method.
		 */
		public void unlockNet() {
			semaphore.release();
		}

		/**
		 * This method returns external net hidden layer dimension,
		 * plausibly higher than real net hidden layer size.
		 * @return wrapped net hidden layer dimension.
		 */
		public int getExtSize() {
			return ext;
		}

		/**
		 * This method set desired external net hidden layer dimension.
		 * @param ext desiret external net hidden layer size.
		 */
		public void setExtSize(int ext) {
			int oldext = this.ext;
			if((ext > 0) && (ext != oldext)) {
				this.ext = ext;
				net.setWrapSize(ext);
			}
		}

		/**
		 * Method that sets availability of the net.
		 * @param stat net is available for end user only if stat is
		 * 	true.
		 */
		public void setNetStatus(boolean stat) {
			available = stat;
		}

		/**
		 * Method that retrieves net availability.
		 * @return net avilability.
		 */
		public boolean isNetAvailable() {
			return available;
		}

		/**
		 * Method that returns scale of managed net.
		 * @return scale value.
		 */
		public int getScale() {
			return scale;
		}

		/**
		 * Method that sets scale for the managed net.
		 * @param scale new scale value.
		 */
		public void setScale(int scale) {
			this.scale = scale;
		}
	}

	/**
	 * NetManager unique constructor, initializes netList with an empty list
	 * and sets the net id counter to zero.
	 */
	public NetManager() {
		netIDCounter = 0;
		netList = new HashMap<Integer, NetHandler>();
	}

	/**
	 * This method retrieves some informations about available nets.
	 * @return array of array that contains informations about available
	 * 	nets.
	 */
	public Object[][] getAvailableNetsList() {
		Iterator<Integer> ii = netList.keySet().iterator();
		Vector<Object[]> list = new Vector<Object[]>();
		while(ii.hasNext()) {
			Integer ival = ii.next();
			NetHandler nh = netList.get(ival);
			if(nh.isNetAvailable()) {
				Object[] values = nh.retrieveLimitedNetInfo();
				Object[] cvalues = new Object[1 + values.length];
				cvalues[0] = ival;
				for(int iter = 0; iter < values.length; iter++)
					cvalues[iter+1] = values[iter];
				list.addElement(cvalues);
			}
		}
		Object[] reflist = list.toArray();
		Object[][] alist = new Object[reflist.length][];
		for(int iter = 0; iter < reflist.length; iter++)
			alist[iter] = (Object[]) reflist[iter];
		return alist;
	}

	/**
	 * This method retrieves full informations about available nets.
	 * @return array of array that contains informations about nets.
	 */
	public Object[][] getManagedNetsList() {
		Iterator<Integer> ii = netList.keySet().iterator();
		Vector<Object[]> list = new Vector<Object[]>();
		while(ii.hasNext()) {
			Integer ival = ii.next();
			NetHandler nh = netList.get(ival);
			Object[] values = nh.retrieveFullNetInfo();
			Object[] cvalues = new Object[1 + values.length];
			cvalues[0] = ival;
			for(int iter = 0; iter < values.length; iter++)
				cvalues[iter+1] = values[iter];
			list.addElement(cvalues);
		}
		Object[] reflist = list.toArray();
		Object[][] mlist = new Object[reflist.length][];
		for(int iter = 0; iter < reflist.length; iter++)
			mlist[iter] = (Object[]) reflist[iter];
		return mlist;
	}

	/**
	 * This method retrieves full informations about a specific net.
	 * @param id net identifier.
	 * @return array that contains informations about a specific net.
	 */
	public Object[] getNetInformations(int id) {
		Object[] cvalues = null;
		Integer iid = new Integer(id);
		Iterator<Integer> ii = netList.keySet().iterator();
		while((ii.hasNext()) && (cvalues == null)) {
			Integer ival = ii.next();
			if(ival.equals(iid)) {
				NetHandler nh = netList.get(ival);
				Object[] values = nh.retrieveFullNetInfo();
				cvalues = new Object[1 + values.length];
				cvalues[0] = ival;
				for(int iter = 0; iter < values.length; iter++)
					cvalues[iter+1] = values[iter];
			}
		}
		return cvalues;
	}

	/**
	 * Method that permits to load a new net from file.
	 * @param filename file on the filesystem.
	 * @throws NetLoadingException thrown when error occurs loading net.
	 * @return net associated identifier.
	 */
	public int loadNet(String filename) throws NetLoadingException {
		Parser pref = null;
		try {
			pref = new Parser(new FileReader(filename));
			if(pref != null) pref.parse();
			netList.put(new Integer(++netIDCounter), new NetHandler(pref.getNet(), filename));
			return netIDCounter;
		} catch(Exception excp) {
			throw new NetLoadingException(excp.getMessage());
		}
	}

	/**
	 * Method that permits to reload a net previously loaded. It is useful
	 * when users modifies net description files.
	 * @param id net identifier.
	 * @throws NetLoadingException this method does not catch this exception
	 * 	from loadNet method.
	 * @return net associated identifier.
	 */
	public int reloadNet(int id) throws NetLoadingException {
		NetHandler nh = netList.remove(new Integer(id));
		return loadNet(nh.getFilename());
	}

	/**
	 * Method that permits to unload a net previously loaded.
	 * @param id net identifier.
	 */
	public void unloadNet(int id) {
		netList.remove(new Integer(id));
	}

	/**
	 * This method allows to set the net status (availability).
	 * @param id   net identifier.
	 * @param stat net will be available if sta is true.
	 */
	public void setNetStatus(int id, boolean stat) {
		NetHandler nh = netList.get(new Integer(id));
		if(nh != null)
			nh.setNetStatus(stat);
	}

	/**
	 * This method allows to set wrapped net hidden layer size.
	 * @param id  net identifier.
	 * @param ext proposed external size.
	 */
	public void setNetExtSize(int id, int ext) {
		NetHandler nh = netList.get(new Integer(id));
		if(nh != null) {
			nh.lockNet();
			nh.setExtSize(ext);
			nh.unlockNet();
		}
	}

	/**
	 * Method that permits to retrieve net references. This controls net
	 * access throughout NetHandler class and returns only if net is
	 * available (lock-after control).
	 * @param id net identifier.
	 * @return wrapped net reference.
	 */
	public NetIFace lockNet(int id) {
		NetIFace net = null;
		NetHandler nh = netList.get(new Integer(id));
		if(nh != null) {
			net = nh.lockNet();
			if(!nh.isNetAvailable()) {
				net = null;
				nh.unlockNet();
			}
		}
		return net;
	}

	/**
	 * Method that permits to release previously locked net. This controls
	 * net access throughout NetHandler class.
	 * @param id net identifier.
	 */
	public void unlockNet(int id) {
		NetHandler nh = netList.get(new Integer(id));
		if(nh != null) nh.unlockNet();
	}

	/**
	 * Method that returns the scale associated with a specific net.
	 * @param id net identifier.
	 * @return net associated scale.
	 */
	public int getNetScale(int id) {
		NetHandler nh = netList.get(new Integer(id));
		if(nh != null)
			return nh.getScale();
		else return 0;
	}

	/**
	 * Method that permits to set the scale associated with a specific net.
	 * @param id    net identifier.
	 * @param scale new scale value.
	 */
	public void setNetScale(int id, int scale) {
		NetHandler nh = netList.get(new Integer(id));
		if(nh != null) {
			nh.lockNet();
			nh.setScale(scale);
			nh.unlockNet();
		}
	}

	/**
	 * Method that sets values for a specified net from an Object values
	 * array.
	 * @param values Object values array.
	 * @return true if net is updated, false otherwise.
	 */
	public boolean updateNet(Object[] values) {
		NetHandler nh = null;
		if((values != null) && (values[0] instanceof Integer))
			nh = netList.get(((Integer) values[0]).intValue());
		else return false;
		if((nh != null) && (values.length >= 4)) {
			nh.lockNet();
			if(values[values.length-1] instanceof Boolean)
				nh.setNetStatus(((Boolean) values[values.length-1]).booleanValue());
			if(values[values.length-2] instanceof Integer)
				nh.setScale(((Integer) values[values.length-2]).intValue());
			if(values[values.length-3] instanceof Integer)
				nh.setExtSize(((Integer) values[values.length-3]).intValue());
			nh.unlockNet();
		} else return false;;
		return true;
	}
}
