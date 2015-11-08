/**
 * Calculator is the class that the server uses to calculate values.
 *
 * @author Michele Caini
 */

package nnsec.client;

import java.math.BigInteger;
import java.math.BigDecimal;
import java.rmi.RemoteException;
import nnsec.cryptosystem.Cryptosystem;
import nnsec.cryptosystem.Mapper;

/**
 * Calculator is the class that client pass to the server (as a remote
 * reference) so that the server can use this class to compute some values that
 * it is not able to compute himself.
 */
public class Calculator implements CalculatorIFace {

	/** Protocol related scale. */
	private int scale;

	/** Time spent on this unit. */
	private long gap;

	/**
	 * Exception thrown when trying to ask for a function, and then this
	 * function doesn't exist.
	 */
	static public class UnsupportedFunctionException extends RemoteException {

		/**
		 * Constructs an UnsupportedFunctionException.
		 */
		public UnsupportedFunctionException() { super(); }

		/**
		 * Constructs an UnsupportedFunctionException with the specified
		 * detail message.
		 * @param msg the detail message.
		 */
		public UnsupportedFunctionException(String msg) { super(msg); }
	}

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
		public ProtocolException(String msg) { super(msg); }
	}
	
	/**
	 * Calculator constructor.
	 * @param scale protocol related scale.
	 */
	public Calculator(int scale) throws RemoteException {
		this.scale = scale;
		gap = 0;
	}

	/**
	 * Simply: f(x) = sign(x)
	 * It returns the sign of x value.
	 * @param value value is the x parameter.
	 * @return result of f(x) (see above).
	 */
	public double sign(double value) {
		if(value == 0) return value;
		else if(value < 0) return -1;
		else return 1;
	}

	/**
	 * Simply: f(x) = 1 / ( 1 + e<SUP>x</SUP>)
	 * @param value value is the x parameter.
	 * @return result of f(x) (see above).
	 */
	public double sigmoid(double value) {
		return 1 / ( 1 + Math.exp(-1 * value)); 
	}

	/**
	 * This method is the one that really computes the value with the
	 * specified function, no matter which kind of operation is asked for.
	 * @param value    the value to be computed.
	 * @param function function to be used.
	 * @return computed value.
	 * @throws UnsupportedFunctionException thrown as a consequence of
	 * 	unsupported function or null function pointer.
	 */
	private double computeValue(double value, String function) throws UnsupportedFunctionException {
		if(function == null)
			throw new UnsupportedFunctionException("unusable net: function not defined");
		double cvalue;
		if(function.equals("sign")) cvalue = sign(value);
		else if(function.equals("sigmoid")) cvalue = sigmoid(value);
		else throw new UnsupportedFunctionException("function " + function + " not available ...");
		return cvalue;
	}

	/**
	 * Implementation of computeValue method from CalculatorIFace, this
	 * method computes submitted value with specified function and returns
	 * the result. In fact, it decrypts value, computes it and then encrypts
	 * back before to return. This method is used by the worker like an
	 * activation function.
	 * @param value    ciphered form of the value to be computed.
	 * @param function function to be used.
	 * @return chipered computed value.
	 * @throws ProtocolException thrown as a consequence of errors during
	 * 	encryption/decryption session.
	 * @throws UnsupportedFunctionException thrown as a consequence of
	 * 	unsupported function or null function pointer.
	 */
	public String computeValue(String value, String function) throws ProtocolException, UnsupportedFunctionException {
		try {
			long pre = System.currentTimeMillis();
			Cryptosystem cs = Cryptosystem.getInstance();
			Mapper mapper = new Mapper(scale, cs.getPublicKey().getModulus().pow(cs.getSParameter()));
			BigDecimal bdv = mapper.deQuantization(mapper.positiveIntegerToReal(cs.decrypt(new BigInteger(value))));
			BigInteger biv = cs.encrypt(mapper.realToPositiveInteger(computeValue(bdv.doubleValue(), function)));
			long post = System.currentTimeMillis();
			gap += post - pre;
			return biv.toString();
		} catch (Cryptosystem.MessageTooBigException mtbe) {
			throw new ProtocolException(mtbe.getMessage());
		}
	}

	/**
	 * Implementation of mangleValue method from CalculatorIFace, this
	 * method computes submited value with specified function and returns
	 * the result. In fact, it decrypts value, computes it and then encrypts
	 * back before to return. This method is udes by the worker like an
	 * output function.
	 * @param value    ciphered form of the value to be computed.
	 * @param function function to be used.
	 * @return chipered computed value.
	 * @throws ProtocolException thrown as a consequence of errors during
	 * 	encryption/decryption session.
	 * @throws UnsupportedFunctionException thrown as a consequence of
	 * 	unsupported function or null function pointer.
	 */
	public String mangleValue(String value, String function) throws ProtocolException, UnsupportedFunctionException {
		try {
			long pre = System.currentTimeMillis();
			Cryptosystem cs = Cryptosystem.getInstance();
			Mapper mapper = new Mapper(scale, cs.getPublicKey().getModulus().pow(cs.getSParameter()));
			BigDecimal bdv = mapper.positiveIntegerToReal(cs.decrypt(new BigInteger(value)));
			BigInteger biv = cs.encrypt(mapper.realToPositiveInteger(computeValue(bdv.doubleValue(), function)));
			long post = System.currentTimeMillis();
			gap += post - pre;
			return biv.toString();
		} catch (Cryptosystem.MessageTooBigException mtbe) {
			throw new ProtocolException(mtbe.getMessage());
		}
	}

	/**
	 * Returns the size of gap, that is, the time spent on this unit.
	 * @return the gap
	 */
	public long getOnBoardGap() {
		return gap;
	}
}
