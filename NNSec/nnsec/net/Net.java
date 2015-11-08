/**
 * Net, or rather neural network, is the fundamental class of the system.
 * Instances of this class represent the neural networks that server exports and
 * that clients want to use. So, without neural networks, server can shuts up
 * and clients can have holidays!
 *
 * @author Michele Caini
 */

package nnsec.net;

import java.util.Iterator;
import java.util.Vector;

/**
 * Net class represents main implementation of NetIFace. Instances of this class
 * are neural networks that server exports to the clients and clients want to
 * use.
 */
public class Net implements NetIFace {

	/** Neural network name. */
	private String name;

	/** Neural network description. */
	private String desc;

	/** Neural network default associated activation function. */
	private String actvFunc;

	/** Neural network default associated output function. */
	private String outFunc;

	/** Neural network input layer. */
	private Vector<Node> inNodes;

	/** Neural network hidden layer. */
	private Vector<Node> hiddNodes;

	/** Neural network output layer. */
	private Vector<Node> outNodes;

	/**
	 * Default constructor.
	 */
	public Net() {
		name = new String("No Name");
		desc = new String("No Description");
		actvFunc = null;
		outFunc = null;
		inNodes = new Vector<Node>();
		hiddNodes = new Vector<Node>();
		outNodes = new Vector<Node>();
	}

	/**
	 * Method that permits to set the name of the neural network.
	 * @param name proposed name.
	 */
	public void setName(String name) {
		this.name = name;
	}

	/**
	 * Method that permits to know which is the name of the neural network.
	 * @return neural network name.
	 */
	public String getName() {
		return name;
	}

	/**
	 * Method that permits to set a description for the neural network.
	 * @param desc proposed description.
	 */
	public void setDescription(String desc) {
		this.desc = desc;
	}

	/**
	 * Method that permits to know which is the description of the neural
	 * network.
	 * @return neural network description.
	 */
	public String getDescription() {
		return desc;
	}

	/**
	 * Method that permits to know if the neural network has an associated
	 * default activation function.
	 * @return true if the neural network has an associated default
	 * 	activation function, false otherwise.
	 */
	public boolean hasDefaultActvFunction() {
		return (actvFunc != null);
	}

	/**
	 * Method that permits to set a default associated activation function
	 * for the neural network.
	 * @param func proposed function.
	 */
	public void setDefaultActvFunction(String func) {
		actvFunc = func;
	}

	/**
	 * Method that permits to know which is the neural network default
	 * associated activation function.
	 * @return neural network default associated activation function.
	 */
	public String getDefaultActvFunction() {
		return actvFunc;
	}

	/**
	 * Method that permits to know if the neural network has an associated
	 * default output function.
	 * @return true if the neural network has an associated default
	 * 	output function, false otherwise.
	 */
	public boolean hasDefaultOutFunction() {
		return (outFunc != null);
	}

	/**
	 * Method that permits to set a default associated output function
	 * for the neural network.
	 * @param func proposed function.
	 */
	public void setDefaultOutFunction(String func) {
		outFunc = func;
	}

	/**
	 * Method that permits to know which is the neural network default
	 * associated output function.
	 * @return neural network default associated output function.
	 */
	public String getDefaultOutFunction() {
		return outFunc;
	}

	/**
	 * Method that permits to add a node to the input layer of the neural
	 * network.
	 * @param node node reference.
	 */
	public void addInputNode(Node node) {
		inNodes.addElement(node);
	}

	/**
	 * Method that permits to know the input layer size of the neural
	 * network.
	 * @return neural network input layer size.
	 */
	public int getInputLayerSize() {
		return inNodes.size();
	}

	/**
	 * Method that permits to get an iterator throughout the nodes that
	 * compose the neural network input layer.
	 * @return iterator reference.
	 */
	public Iterator<Node> getInputLayerIterator() {
		return inNodes.iterator();
	}

	/**
	 * Method that permits to add a node to the hidden layer of the neural
	 * network.
	 * @param node node reference.
	 */
	public void addHiddenNode(Node node) {
		hiddNodes.addElement(node);
	}

	/**
	 * Method that permits to know the hiddenput layer size of the neural
	 * network.
	 * @return neural network hiddenput layer size.
	 */
	public int getHiddenLayerSize() {
		return hiddNodes.size();
	}

	/**
	 * Method that permits to get an iterator throughout the nodes that
	 * compose the neural network hidden layer.
	 * @return iterator reference.
	 */
	public Iterator<Node> getHiddenLayerIterator() {
		return hiddNodes.iterator();
	}

	/**
	 * Method that permits to add a node to the output layer of the neural
	 * network.
	 * @param node node reference.
	 */
	public void addOutputNode(Node node) {
		outNodes.addElement(node);
	}

	/**
	 * Method that permits to know the output layer size of the neural
	 * network.
	 * @return neural network output layer size.
	 */
	public int getOutputLayerSize() {
		return outNodes.size();
	}

	/**
	 * Method that permits to get an iterator throughout the nodes that
	 * compose the neural network output layer.
	 * @return iterator reference.
	 */
	public Iterator<Node> getOutputLayerIterator() {
		return outNodes.iterator();
	}
}
