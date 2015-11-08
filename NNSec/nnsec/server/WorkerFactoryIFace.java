/**
 * This interface is useful to turn WorkerFactory class into a remote class.
 *
 * @author Michele Caini
 */

package nnsec.server;

import java.rmi.Remote;
import java.rmi.RemoteException;

/**
 * WorkerFactoryIFace interface contains methods that will be exported to the
 * remote users.
 */
public interface WorkerFactoryIFace extends Remote {

	/**
	 * This method permits to the clients to retrieve a private worker, so
	 * that they can exploits the protocol.
	 * @return worker reference.
	 * @throws RemoteException default exception, as indicated in the rmi
	 * 	reference manual.
	 */
	public WorkerIFace getWorker() throws RemoteException;
}
