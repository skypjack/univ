/**
 * This interface is useful to turn Calculator class into a remote class.
 *
 * @author Michele Caini
 */

package nnsec.client;

import java.rmi.Remote;
import java.rmi.RemoteException;

/**
 * CalculatorIFace interface contains methods that will be exported to the
 * remote users.
 */
public interface CalculatorIFace extends Remote {

	/**
	 * This method should computes and returns value using indicated
	 * function, as activation function.
	 * @param value    value to be computed.
	 * @param function function to be used.
	 * @return computed value.
	 * @throws RemoteException default exception, as indicated in the rmi
	 * 	reference manual.
	 */
	public String computeValue(String value, String function) throws RemoteException;

	/**
	 * This method should computes and returns value using indicated
	 * function, as output function.
	 * @param value    value to be computed.
	 * @param function function to be used.
	 * @return computed value.
	 * @throws RemoteException default exception, as indicated in the rmi
	 * 	reference manual.
	 */
	public String mangleValue(String value, String function) throws RemoteException;
}
