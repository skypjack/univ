/**
 * This interface is useful to turn Worker class into a remote class.
 *
 * @author Michele Caini
 */

package nnsec.server;

import java.rmi.Remote;
import java.rmi.RemoteException;
import nnsec.client.Calculator_Stub;

/**
 * WorkerIFace interface contains methods that will be exported to the remote
 * users.
 */
public interface WorkerIFace extends Remote {
	
	/**
	 * Method that permits to lock a neural network for the use.
	 * @param id neural network reference.
	 * @return true if the net exists, false otherwise.
	 * @throws RemoteException default exception, as indicated in the rmi
	 * 	reference manual.
	 */
	public boolean lockNet(int id) throws RemoteException;

	/**
	 * Method that permits to unlock a neural network previously locked.
	 * @throws RemoteException default exception, as indicated in the rmi
	 * 	reference manual.
	 */
	public void unlockNet() throws RemoteException;

	/**
	 * Method that permits to retrieve an upper bound of the sum of neural
	 * network weights.
	 * @return upper bound of the sum of neural network weights.
	 * @throws RemoteException default exception, as indicated in the rmi
	 * 	reference manual.
	 */
	public int getWeightsUpperBound() throws RemoteException;

	/**
	 * Method that permits to retrieve the neural network associated scale.
	 * @return neural network associated scale.
	 * @throws RemoteException default exception, as indicated in the rmi
	 * 	reference manual.
	 */
	public int getNetScale() throws RemoteException;

	/**
	 * Method that permits to set up the remote cryptosystem environment.
	 * @param puk    public key value.
	 * @param svalue value of the cryptosystem s-parameter.
	 * @throws RemoteException default exception, as indicated in the rmi
	 * 	reference manual.
	 */
	public void setCryptoEnvironment(String puk, int svalue) throws RemoteException;

	/**
	 * Method that permits to retrieve outputs from the locked neural network
	 * (if there is one).
	 * @param inputs chipered inputs, in a specific syntax.
	 * @param cmif   reference to this class, useful to exploit the
	 * 	protocol.
	 * @return chipered outputs, in a specific syntax.
	 * @throws RemoteException default exception, as indicated in the rmi
	 * 	reference manual.
	 */
	public String[] getOutputs(String[] inputs, Calculator_Stub cmif) throws RemoteException;

	/**
	 * Method that permits to retrieve informations about available nets
	 * from the server.
	 * @return available nets informations, in a specific syntax.
	 * @throws RemoteException default exception, as indicated in the rmi
	 * 	reference manual.
	 */
	public Object[][] getNetsList() throws RemoteException;

	/**
	 * Method that permits to know the time spent by a Worker during
	 * last transition (useful for benchmark purpose).
	 * @return on-board Worker gap.
	 * @throws RemoteException default exception, as indicated in the rmi
	 *      reference manual.
	 */
	public long getWorkerGap() throws RemoteException;
}
