/**
 * Mapper class maps values from a domain to another one, quantizes values and
 * combines this kind of operations, so that it can transforms real values to
 * positive integer values and vicerversa.
 * Useful into the proposed protocol because it permits to use real values as
 * inputs and as neural network weight values.
 *
 * @author Michele Caini
 */

package nnsec.cryptosystem;

import java.math.BigInteger;
import java.math.BigDecimal;

/**
 * This class permits to transform real values to positive integer values and
 * viceversa, quantizes values and maps them from a domain to another one
 * combining above discussed operations.
 */
public class Mapper {

	/** Value used in the quantization process. */
	private int scale;

	/** Interval of valid positive/negative integer values. */
	private BigInteger gap;

	/** Max positive integer value allowed (approximately gap / 2). */
	private BigInteger peak;

	/**
	 * Mapper constructor.
	 * @param scale scale value (used in the quantization process).
	 * @param gap   interval of valid positive/negative integer values.
	 */
	public Mapper(int scale, BigInteger gap) {
		this.scale = scale;
		this.gap = gap;
		peak = gap.subtract(BigInteger.ONE).divide(new BigInteger("2"));
	}

	/**
	 * Method that permits to map a value from a domain to another one.
	 * @param value source value.
	 * @return mapped value.
	 */
	public BigInteger mapTo(BigInteger value) {
		BigInteger res = value;
		if(res.signum() == -1)
			res = res.add(gap);
		return res;
	}

	/**
	 * Method that permits to map a value from a domain to another one.
	 * @param value source value.
	 * @return mapped value.
	 */
	public BigInteger mapFrom(BigInteger value) {
		BigInteger res = value;
		if(res.compareTo(peak) == 1)
			res = res.subtract(gap);
		return res;
	}

	/**
	 * Method that permits to quantize a real value to an integer one.
	 * @param real real value to be quantized.
	 * @return quantized value.
	 */
	public BigInteger quantization(BigDecimal real) {
		return real.scaleByPowerOfTen(scale).toBigInteger();
	}

	/**
	 * Method that permits to de-quantize an integer value to a real one.
	 * @param value integer value to be quantized.
	 * @return de-quantized value.
	 */
	public BigDecimal deQuantization(BigInteger value) {
		return deQuantization(new BigDecimal(value));
	}

	/**
	 * Method that permits to de-quantize an integer value to a real one.
	 * @param value integer value to be quantized.
	 * @return de-quantized value.
	 */
	public BigDecimal deQuantization(BigDecimal value) {
		BigDecimal res = value;
		res = res.scaleByPowerOfTen(-1*scale);
		res = res.stripTrailingZeros();
		return res;
	}

	/**
	 * Method that permits to convert a real value to a positive integer
	 * one, with a quantization step followed by a map step.
	 * @param real real value to be converted.
	 * @return converted value.
	 */
	public BigInteger realToPositiveInteger(double real) {
		return realToPositiveInteger(new BigDecimal(real));
	}

	/**
	 * Method that permits to convert a real value to a positive integer
	 * one, with a quantization step followed by a map step.
	 * @param real real value to be converted.
	 * @return converted value.
	 */
	public BigInteger realToPositiveInteger(BigDecimal real) {
		return mapTo(quantization(real));
	}

	/**
	 * Method that permits to convert a positive integer value to a real
	 * one, with a map step followed by a de-quantization step.
	 * @param value positive integer value to be converted.
	 * @return converted value.
	 */
	public BigDecimal positiveIntegerToReal(BigInteger value) {
		return deQuantization(mapFrom(value));
	}
}
