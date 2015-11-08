/**
 * Modified version of Cryptosystem.java from Jurik's homepage.
 * You can retrieve the original one from:
 *    http://www.daimi.au.dk/~jurik/research.html
 *
 * @author Michele Caini
 */

package nnsec.cryptosystem;

import java.math.BigInteger;
import java.util.Random;

/**
 * Cryptosystem class manages public/private key, encrypt/decrypt data and
 * performs basic homomorphic operations.
 */
public class Cryptosystem {
	
	/** Cryptosystem unique instance. */
	private static Cryptosystem cs = null;
	
	/** Private key. */
	private PrivateKey prk;
	
	/** Public key. */
	private PublicKey puk;
	
	/** Plaintext size related parameter useful for encryption session. */
	private int sParam;

	/**
	 * Exception thrown when trying to encrypt or decrypt a message with a
	 * fixed s, and then message or ciphertext exceeds the modulus size.
	 */
	static public class MessageTooBigException extends Exception {

		/**
		 * Constructs a MessageTooBigException.
		 */
		public MessageTooBigException () { super (); }

		/**
		 * Constructs a MessageTooigException with the specified detail
		 * message.
		 * @param msg the detail message.
		 */
		public MessageTooBigException (String msg) { super (msg); }
	}

	/**
	 * Private key class that handles private information and decrypts
	 * message.
	 */
	private class PrivateKey {
		
		/** Private values */
		private BigInteger p, q;
		
		/** Constructor that generates a 512 bit key. */
		public PrivateKey () {
			this (512, 1000);
		}
		
		/**
		 * Constructor used for setting up the Private key.
		 * @param bits   number of bits of the modulus.
		 */
		public PrivateKey (int bits) {
			this (bits, 1000);
		}
		
		/**
		 * Constructor used for setting up the Private key.
		 * @param bits       number of bits of the modulus.
		 * @param certainty  p, q will be not primes with
		 * 	chance < 2^{-certainty}.
		 */
		public PrivateKey (int bits, int certainty) {
			Random rand = new Random ();
			p = new BigInteger (bits / 2, certainty, rand);
			q = new BigInteger (bits / 2, certainty, rand);
			while (q.compareTo (p) == 0)
				q = new BigInteger (bits / 2, certainty, rand);
		}
		
		/**
		 * Method for extracting the modulus (which is the public key)
		 * @return corresponding to this private key.
		 */
		public BigInteger getModulus () {
			return p.multiply (q);
		}
		
		/**
		 * Method for decrypting with surrounding private key. This uses
		 * a specified s, and will return a wrong value if it is not big
		 * enough.
		 * @param cip the ciphertext.
		 * @param s   the size of s used in the decryption,
		 *	n<SUP>s+1</SUP> &gt; cip or an exception is
		 *	thrown.
		 * @return the resulting message.
		 */
		public BigInteger decrypt (BigInteger cip, int s) {
			/* For holding intermidiate results. */
			BigInteger temp;
			/* Used for when a BigInteger counter is need. */
			BigInteger count;
			/* Entry i will have n^{i+1}. */
			BigInteger[] n = new BigInteger[s+1];
			n[0] = getModulus();
			for(int i = 0; i < s; i++)
				n[i+1] = n[i].multiply (n[0]);
			BigInteger ONE = BigInteger.ONE;
			/* Find lambda so we can find d. */
			/* (p - 1)(q - 1) */
			BigInteger p1_q1 = (p.subtract (ONE)).multiply (q.subtract (ONE));
			BigInteger lambda = p1_q1.divide ((p.subtract (ONE)).gcd (q.subtract (ONE)));
			/* Find d so we can decrypt. */
			BigInteger d;
			d = lambda.multiply (lambda.modInverse (n[s-1]));
			/* Decrypt cip. */
			/* Cip raised to power d. */
			BigInteger cip_d;
			/* Array of L values, index is -1. */
			BigInteger[] L = new BigInteger[s];
			/* Common mult values in computations. */
			BigInteger[] mult = new BigInteger[s-1];
			/* The resulting message. */
			BigInteger msg;
			cip_d = cip.modPow (d, n[s]);
			/* L[i] = ((c^d mod n^{i+2}) - 1) / n */
			for (int i = 0; i < s; i++)
				L[i] = ((cip_d.mod (n[i+1])).subtract (ONE)).divide (n[0]);
			 /* used to hold (i+1)! */
			temp = ONE;
			count = ONE;
			for (int i = 1; i < s; i++) {
				/* mult[i] = n^{i+1} / (i+2)! mod n^s   (n^s, so it can be reduced to all n^i) */
				count = count.add (ONE);
				temp = temp.multiply (count);
				mult[i-1] = (n[i-1].multiply (temp.modInverse (n[s-1]))).mod (n[s-1]);
			}
			/* Temp values. */
			BigInteger t1, t2; 
			msg = null;
			for (int j = 1; j <= s; j++) {
				t1 = L[j-1];
				t2 = msg;
				for (int k = 2; k <= j; k++) {
				msg = msg.subtract (ONE);
				t2 = (t2.multiply (msg)).mod (n[j-1]);
				t1 = (t1.subtract (t2.multiply (mult[k-2].mod (n[j-1])))).mod (n[j-1]);
				}
				msg = t1;
			}
			return msg;
		}
	}

	/**
	 * Public key that handles public information and encrypts message. This
	 * is a static class because it can be instantiated by third party that
	 * wants to communicate with end user.
	 */
	static public class PublicKey {
		
		/** The public values, which is the public key. */
		protected BigInteger n;
		
		/**
		 * Constructor used for setting up the Public key.
		 * @param n   a large composite number corresponding to some
		 * 	private key.
		 */
		public PublicKey (BigInteger n) {
			this.n = n;
		}
		
		/**
		 * Method for extracting the modulus (which is the public key).
		 * @return corresponding to this private key.
		 */
		public BigInteger getModulus () {
			return n;
		}
		
		/**
		 * Method that encrypts a message using the surrounding public
		 * key.
		 * @param msg message to be encrypted.
		 * @param s   size of s used in the decryption,
		 * 	n<SUP>s</SUP> &gt; msg or an exception is thrown.
		 * @return encryption of the message msg.
		 * @throws MessageTooBigException used in case of:
		 * 	msg &gt; n<SUP>s</SUP>.
		 */
		public BigInteger encrypt (BigInteger msg, int s) throws MessageTooBigException {
			BigInteger n_s = n.pow (s);
			if (msg.compareTo (n_s) >= 0)
				throw new MessageTooBigException("message to big for encryption with " + s);
			BigInteger n_s1 = n.multiply (n_s);
			// Finds a random value and makes sure it's a good random value.
			Random rand = new Random ();
			BigInteger r = new BigInteger (n_s1.bitLength (), rand);
			while ((r.compareTo (n_s1) >= 0) ||
					(r.compareTo (BigInteger.ZERO) == 0) ||
					(BigInteger.ONE.compareTo (r.gcd (n)) < 0))
				r = new BigInteger (n_s1.bitLength (), rand);
			// The result is calculated.
			BigInteger res = BigInteger.ONE;
			res = res.add(n).modPow(msg, n_s1);
			res = res.multiply (r.modPow (n_s, n_s1)).mod (n_s1);
			return res;
		}
	}
	
	/**
	 * Private contructor, useful for singleton pattern.
	 * No one can builds Cryptosystem class.
	 */
	private Cryptosystem() {
		prk = new PrivateKey();
		puk = new PublicKey(prk.getModulus());
		sParam = 1;
	}
	
	/**
	 * Method that calculates a new pair of private/public keys.
	 * @param bits       number of bits of the modulus.
	 * @param certainty  p, q will be not primes with
	 * 	chance < 2^{-certainty}.
	 */
	public void generateKeys (int bits, int certainty) {
		prk = new PrivateKey(bits, certainty);
		puk = new PublicKey(prk.getModulus());
	}
	
	/**
	 * Method that calculates a new pair of private/public keys.
	 * @param bits number of bits of the modulus.
	 */
	public void generateKeys (int bits) {
		prk = new PrivateKey(bits);
		puk = new PublicKey(prk.getModulus());
	}
	
	/**
	 * Method that calculates a new pair of private/public keys.
	 */
	public void generateKeys () {
		prk = new PrivateKey();
		puk = new PublicKey(prk.getModulus());
	}
	
	/**
	 * Method that returns the public key.
	 * @return the public key.
	 */
	public PublicKey getPublicKey() {
		return puk;
	}
	
	/**
	 * Method that permits to set s-parameter for encryption/decryption
	 * session.
	 * @param s s-parameter value.
	 */
	public void setSParameter(int s) {
		sParam = s;
	}
	
	/**
	 * Method that permits to retrieve s-parameter value.
	 * @return s-parameter value.
	 */
	public int getSParameter() {
		return sParam;
	}
	
	/**
	 * Method that decrypts a ciphertext with the internal private key.
	 * @param cip message to be decrypted.
	 * @return decryption of message msg.
	 */
	public BigInteger decrypt(BigInteger cip) {
		return prk.decrypt(cip, sParam);
	}
	
	/**
	 * Method that encrypts a plaintext with the internal public key.
	 * @param msg the message to be encrypted.
	 * @return encryption of message msg.
	 */
	public BigInteger encrypt(BigInteger msg) throws MessageTooBigException {
		return puk.encrypt(msg, sParam);
	}
	
	/** 
	 * Method that returns an instance of Cryptosystem class. Users can be
	 * sure that only one Cryptosystem object will be present at a time
	 * (thanks to the singleton pattern).
	 * @return unique instance of Cryptosystem class.
	 */
	public static Cryptosystem getInstance() {
		if(cs == null)
			cs = new Cryptosystem();
		return cs;
	}
	
	/**
	 * Method that combines two ciphertexts such that resulting ciphertext
	 * encrypts the sum of the two plaintexts in the two given encryptions.
	 * @param puk  public key
	 * @param cip1 first ciphertext
	 * @param cip2 second ciphertext
	 * @param s    value of s that should be used when handling the
	 * 	ciphertexts
	 * @return combined ciphertext
	 */
	public static BigInteger combineCiphertexts(PublicKey puk, BigInteger cip1, BigInteger cip2, int s) {
		return cip1.multiply(cip2).mod(puk.getModulus().pow(s+1));
	}
	
	/**
	 * Method that raises a ciphertext with a plaintext token such that
	 * resulting ciphertext encrypts the multiplication of the two
	 * plaintexts.
	 * @param puk public key
	 * @param cip first ciphertext
	 * @param msg plaintext token
	 * @param s   value of s that should be used when handling the
	 * 	ciphertext
	 * @return raised ciphertext
	 */
	public static BigInteger raiseCiphertext(PublicKey puk, BigInteger cip, BigInteger msg, int s) {
		return cip.modPow(msg, puk.getModulus().pow(s+1));
	}
}
