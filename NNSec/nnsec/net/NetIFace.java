/**
 * NetIFace interface introduces net related objects like nodes and links
 * between nodes and defines the default functions that a neural network should
 * implements.
 *
 * @author Michele Caini
 */

package nnsec.net;

import java.util.Iterator;
import java.util.Vector;
import java.math.BigInteger;

/**
 * NetIFace interface declares basic bricks that users can use to construct
 * more complicate neural network objects and defines standard operations that
 * this kind of objects should implement.
 */
public interface NetIFace {

	/**
	 * Node class is a fundamental piece useful to construct neural
	 * networks. Node objects have a value, a threshold, an associated
	 * function, a group, and a collection of in/out links.
	 */
	static public class Node {

		/** In-links for this node. */
		private Vector<Link> inLinks;
		
		/** Out-links from this node. */
		private Vector<Link> outLinks;

		/** Value associated with this node. */
		private BigInteger value;

		/** If present, threshold is passed to activation function. */
		private double threshold;

		/**
		 * If present, this is the activation function associated with
		 * the node.
		 */
		private String associatedActvFunction;

		/**
		 * If present, this is the output function associated with
		 * the node.
		 */
		private String associatedOutFunction;

		/**
		* Scrambling group ID (layer related value), useful to group together some
		* nodes.
		*/
		private int group;

		/**
		 * Default constructor, it creates a standard unhelpful node.
		 */
		public Node() {
			value = BigInteger.ONE;
			threshold = 0;
			associatedActvFunction = null;
			associatedOutFunction = null;
			group = 0;
			inLinks = new Vector<Link>();
			outLinks = new Vector<Link>();
		}

		/**
		 * Constructs a standard node with the specified group
		 * identifier.
		 * @param group node group identifier.
		 */
		public Node(int group) {
			value = BigInteger.ONE;
			threshold = 0;
			associatedActvFunction = null;
			associatedOutFunction = null;
			this.group = group;
			inLinks = new Vector<Link>();
			outLinks = new Vector<Link>();
		}

		/**
		 * Method that permits to know if node belongs to some groups.
		 * @return true if node belongs to some groups, false otherwise.
		 */
		public boolean hasGroup() {
			return (group != 0);
		}

		/**
		 * This method permits to retrieve the group identifier which
		 * node belongs to.
		 * @return group identifier.
		 */
		public int getGroup() {
			return group;
		}

		/**
		 * Method that permits to retrieve the node value.
		 * @return node value.
		 */
		public BigInteger getValue() {
			return value;
		}

		/**
		 * This method permits to set the node value.
		 * @param value new node value.
		 */
		public void setValue(BigInteger value) {
			this.value = value;
		}

		/**
		 * Method that permits to know if the node has a threshold.
		 * @return true if the node has a threshold, false otherwise.
		 */
		public boolean hasThreshold() {
			return (threshold != 0);
		}

		/**
		 * This method permits to retrieve the node threshold.
		 * @return node threshold.
		 */
		public double getThreshold() {
			return threshold;
		}

		/**
		 * Method that permits to set the node threshold.
		 * @param threshold new node threshold.
		 */
		public void setThreshold(double threshold) {
			this.threshold = threshold;
		}

		/**
		 * This method permits to know if there is an activation
		 * function associated with this node.
		 * @return true if the node has an associated function, false
		 * 	otherwise.
		 */
		public boolean hasAssociatedActvFunction() {
			return (associatedActvFunction != null);
		}

		/**
		 * Method that permits to retrieve the activation function
		 * associated with the node.
		 * @return node associated function.
		 */
		public String getAssociatedActvFunction() {
			return associatedActvFunction;
		}

		/**
		 * This method permits to set the activation function associated
		 * with the node.
		 * @param function new node associated function.
		 */
		public void setAssociatedActvFunction(String function) {
			associatedActvFunction = function;
		}

		/**
		 * This method permits to know if there is an output
		 * function associated with this node.
		 * @return true if the node has an associated function, false
		 * 	otherwise.
		 */
		public boolean hasAssociatedOutFunction() {
			return (associatedOutFunction != null);
		}

		/**
		 * Method that permits to retrieve the output function
		 * associated with the node.
		 * @return node associated function.
		 */
		public String getAssociatedOutFunction() {
			return associatedOutFunction;
		}

		/**
		 * This method permits to set the output function associated
		 * with the node.
		 * @param function new node associated function.
		 */
		public void setAssociatedOutFunction(String function) {
			associatedOutFunction = function;
		}

		/**
		 * Method that permits to add an input link to the node.
		 * @param inLink node input link reference.
		 */
		public void addInputLink(Link inLink) {
			inLinks.addElement(inLink);
		}

		/**
		 * This method permits to add an output link to the node.
		 * @param outLink node output link reference.
		 */
		public void addOutputLink(Link outLink) {
			outLinks.addElement(outLink);
		}

		/**
		 * Method that permits to get an iterator throughout input
		 * link references.
		 * @return input links iterator reference.
		 */
		public Iterator<Link> getInputLinksIterator() {
			return inLinks.iterator();
		}

		/**
		 * This method permits to get an iterator throughout output
		 * link references.
		 * @return output links iterator reference.
		 */
		public Iterator<Link> getOutputLinksIterator() {
			return outLinks.iterator();
		}
	}

	/**
	 * Link class is a fundamental piece useful to construct neural
	 * networks. Link objects have a weight, an output node (node which link
	 * comes from) and an input node (node which link drives to).
	 */
	static public class Link {

		/** Node which link comes from. */
		private Node outNode;

		/** Node which link drives to. */
		private Node inNode;

		/** Weight of the link. */
		private double weight;

		/**
		 * Constructs a link with a complete set of parameters.
		 * @param out    node which link comes from.
		 * @param in     node which link drives to.
		 * @param weight weight of the link.
		 */
		public Link(Node out, Node in, double weight) {
			outNode = out;
			inNode = in;
			this.weight = weight;
		}

		/**
		 * Constructs a link without knowledge about weight.
		 * @param out node which link comes from.
		 * @param in  node which link drives to.
		 */
		public Link(Node out, Node in) {
			outNode = out;
			inNode = in;
			weight = 1;
		}

		/**
		 * Method that permits to get the weight of the link.
		 * @return link associated weight.
		 */
		public double getWeight() {
			return weight;
		}

		/**
		 * Method that permits to know where link comes from.
		 * @return node which link comes from.
		 */
		public Node getOutputNode() {
			return outNode;
		}

		/**
		 * Method that permits to know where link drives to.
		 * @return node which link drives to.
		 */
		public Node getInputNode() {
			return inNode;
		}
	}

	/**
	 * This method is intended to set the name for objects that implement
	 * NetIFace interface.
	 * @param name proposed name.
	 */
	public void setName(String name);

	/**
	 * This method is intended to get the name of the objects that implement
	 * NetIFace interface.
	 * @return objects name.
	 */
	public String getName();

	/**
	 * This method is intended to set the description for objects that
	 * implement NetIFace interface.
	 * @param desc proposed description.
	 */
	public void setDescription(String desc);

	/**
	 * This method is intended to get the description of the objects that
	 * implement NetIFace interface.
	 * @return objects description.
	 */
	public String getDescription();

	/**
	 * This methd is intended to know if objects that implement NetIFace
	 * interface have a default associated activation function.
	 * @return true if exists a default associated activation function,
	 * 	false otherwise.
	 */
	public boolean hasDefaultActvFunction();

	/**
	 * This method is intended to set an associated activation function
	 * for objects that implement NetIFace interface.
	 * @param func proposed function.
	 */
	public void setDefaultActvFunction(String func);

	/**
	 * This method is intended to retrieve the associated activation
	 * function for objects that implemente NetIFace interface.
	 * @return default associated activation function.
	 */
	public String getDefaultActvFunction();

	/**
	 * This methd is intended to know if objects that implement NetIFace
	 * interface have a default associated output function.
	 * @return true if exists a default associated output function,
	 * 	false otherwise.
	 */
	public boolean hasDefaultOutFunction();

	/**
	 * This method is intended to set an associated output function
	 * for objects that implement NetIFace interface.
	 * @param func proposed function.
	 */
	public void setDefaultOutFunction(String func);

	/**
	 * This method is intended to retrieve the associated output
	 * function for objects that implemente NetIFace interface.
	 * @return default associated output function.
	 */
	public String getDefaultOutFunction();

	/**
	 * This method is intended to add e new node to the input layer of
	 * objects that implement NetIFace interface.
	 * @param node new node reference.
	 */
	public void addInputNode(Node node);

	/**
	 * This method is intended to get the input layer size of the objects
	 * that implement NetIFace interface.
	 * @return input layer size.
	 */
	public int getInputLayerSize();

	/**
	 * This method is intended to add e new node to the output layer of
	 * objects that implement NetIFace interface.
	 * @param node new node reference.
	 */
	public void addOutputNode(Node node);

	/**
	 * This method is intended to get the output layer size of the objects
	 * that implement NetIFace interface.
	 * @return output layer size.
	 */
	public int getOutputLayerSize();

	/**
	 * This method is intended to add e new node to the hidden layer of
	 * objects that implement NetIFace interface.
	 * @param node new node reference.
	 */
	public void addHiddenNode(Node node);

	/**
	 * This method is intended to get the hidden layer size of the objects
	 * that implement NetIFace interface.
	 * @return hidden layer size.
	 */
	public int getHiddenLayerSize();

	/**
	 * This method is intended to get an iterator throughout input layer (or
	 * rather, throughout nodes that compose input layer).
	 * @return iterator reference.
	 */
	public Iterator<Node> getInputLayerIterator();

	/**
	 * This method is intended to get an iterator throughout output layer
	 * (or rather, throughout nodes that compose output layer).
	 * @return iterator reference.
	 */
	public Iterator<Node> getOutputLayerIterator();

	/**
	 * This method is intended to get an iterator throughout hidden layer
	 * (or rather, throughout nodes that compose hidden layer).
	 * @return iterator reference.
	 */
	public Iterator<Node> getHiddenLayerIterator();
}
