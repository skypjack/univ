/**
 * Instances of this class are useful to wrap (or rather, to obscure) a net,
 * submitted as net interface reference (of course, it is possible to compose
 * together some nets and nets wrapper).
 *
 * @author Michele Caini
 */

package nnsec.net;

import java.util.Vector;
import java.util.Iterator;

/**
 * This kind of objects wraps around net instances, hiding real
 * structure of the net.
 */
public class NetWrapper implements NetIFace {

	/** Embedded net reference. */
	private NetIFace embedded;
		
	/**
	 * Basket is a pool of nodes that contains real nodes of the net
	 * and fakes additional nodes.
	 */
	private Vector<NetIFace.Node> basket;

	/**
	 * Basket size, this is the sum of real nodes of the net and
	 * fakes additional nodes.
	 */
	private int bSize;

	/**
	 * NetWrapper base constructor, it sets basket size by default
	 * to: net hidden layer size * 2 .
	 * @param embedded net reference.
	 */
	public NetWrapper(NetIFace embedded) {
		this.embedded = embedded;
		bSize = embedded.getHiddenLayerSize();
		basket = new Vector<NetIFace.Node>();
		fillBasket();
		scrambleGroups();
	}

	/**
	 * NetWrapper extended constructor, it sets basket size to the
	 * proposed size only if proposed size is higher than net hidden
	 * layer size.
	 * @param embedded net reference.
	 * @param size     proposed basket size.
	 */
	public NetWrapper(NetIFace embedded, int size) {
		this.embedded = embedded;
		if(size > 0) bSize = size;
		else bSize = 0;
		bSize += embedded.getHiddenLayerSize();
		fillBasket();
		scrambleGroups();
	}

	/**
	 * This method permits to modify hidden layer size of the hidden
	 * referenced net with a new size.
	 * @param nsize number of fake nodes to be added.
	 */
	public void setWrapSize(int nsize) {
		if(nsize > 0)
			bSize = nsize;
		else bSize = 0;
		bSize += embedded.getHiddenLayerSize();
		fillBasket();
		scrambleGroups();
	}

	/**
	 * This method permits to retrieve the embedded net.
	 */
	public NetIFace getEmbedded() {
		return embedded;
	}

	/**
	 * This method scrambles real nodes (the nodes that are member
	 * of net) if they belongs to the same group.
	 */
	public void scrambleGroups() {
		for(int iter = 0; iter < basket.size() - 1; iter++)
			for(int intiter = 0; intiter < basket.size(); intiter++)
				if(basket.elementAt(iter).hasGroup() && basket.elementAt(intiter).hasGroup()
						&& (basket.elementAt(iter).getGroup() == basket.elementAt(intiter).getGroup()))
					if(Math.random() > Math.random())
						basket.set(intiter, basket.set(iter, basket.elementAt(intiter)));
	}

	/**
	 * This method fills the basket using real nodes from net and adding
	 * extra fakes nodes. Real nodes from net are mixed up with
	 * extra fakes nodes.
	 */
	private void fillBasket() {
		int nodes = embedded.getHiddenLayerSize();
		basket = new Vector<NetIFace.Node>();
		int gap = bSize - nodes;
		int lowgroup = -1;
		Iterator<NetIFace.Node> nIter = embedded.getHiddenLayerIterator();
		while((nodes > 0) || (gap > 0)) {
			if(((double)gap / ((double)gap + (double)nodes)) > Math.random()) {
				basket.addElement(new NetIFace.Node(-1));
				gap--;
			} else {
				if(nIter.hasNext()) {
					NetIFace.Node onode = nIter.next();
					if(((onode.hasGroup()) && (lowgroup == -1)) || 
							((onode.hasGroup()) && (onode.getGroup() < lowgroup)))
						lowgroup = onode.getGroup();
					basket.addElement(onode);
				}
				nodes--;
			}
		}
		Vector<NetIFace.Node> fakes = new Vector<NetIFace.Node>();
		int group = (lowgroup != -1)?lowgroup:(lowgroup = 0);
		for(int iter = 0; iter < bSize; iter++) {
			if((basket.get(iter).hasGroup()) && (basket.get(iter).getGroup() == -1)) {
				NetIFace.Node nTmp = new NetIFace.Node(group);
				basket.setElementAt(nTmp, iter);
				fakes.addElement(nTmp);
			} else {
				if(basket.get(iter).hasGroup())
					group = basket.get(iter).getGroup();
			}
		}
		Iterator<NetIFace.Node> fIter = fakes.iterator();
		while(fIter.hasNext()) {
			NetIFace.Node fnode = fIter.next();
			if((!fnode.hasGroup()) || (fnode.getGroup() == lowgroup)) {
				int iprob = 0;
				Iterator<NetIFace.Node> iIter = embedded.getInputLayerIterator();
				while(iIter.hasNext()) {
					if((double)((double)(++iprob) / (double)embedded.getInputLayerSize()) > Math.random()) {
						iprob--;
						NetIFace.Node ilinked = iIter.next();
						NetIFace.Link link = new NetIFace.Link(ilinked, fnode, Math.random());
						ilinked.addOutputLink(link);
						fnode.addInputLink(link);
					} else iIter.next();
				}
			} else {
				boolean linked = false;
				for(int cnt = 0; basket.get(cnt) != fnode; cnt++) {
					if((basket.get(cnt).getGroup() < fnode.getGroup()) && (Math.random() > Math.random())) {
						NetIFace.Node plinked = basket.get(cnt);
						NetIFace.Link link = new NetIFace.Link(plinked, fnode, Math.random());
						plinked.addOutputLink(link);
						fnode.addInputLink(link);
						linked = true;
					}
				}
				if(!linked) {
					int isprob = 0;
					Iterator<NetIFace.Node> isIter = embedded.getInputLayerIterator();
					while(isIter.hasNext()) {
						if((double)((double)(++isprob) / (double)embedded.getInputLayerSize()) > Math.random()) {
							isprob--;
							NetIFace.Node islinked = isIter.next();
							NetIFace.Link slink = new NetIFace.Link(islinked, fnode, Math.random());
							islinked.addOutputLink(slink);
							fnode.addInputLink(slink);
						} else isIter.next();
					}
				}
			}
		}
	}

	/**
	 * Method that permits to set the name of the net.
	 * @param name name of the net.
	 */
	public void setName(String name) {
		embedded.setName(name);
	}

	/**
	 * Method that returns net name.
	 * @return net name.
	 */
	public String getName() {
		return embedded.getName();
	}

	/**
	 * Method that permits to set the description of the net.
	 * @param desc net description.
	 */
	public void setDescription(String desc) {
		embedded.setDescription(desc);
	}

	/**
	 * Method that returns net description.
	 * @return net description.
	 */
	public String getDescription() {
		return embedded.getDescription();
	}

	/**
	 * Method that indicates if the net has a default activation function.
	 * @return true if net has default activation function, false
	 * 	otherwise.
	 */
	public boolean hasDefaultActvFunction() {
		return embedded.hasDefaultActvFunction();
	}

	/**
	 * Method that permits to set a default activation function for the
	 * net.
	 * @param func net default activation function.
	 */
	public void setDefaultActvFunction(String func) {
		embedded.setDefaultActvFunction(func);
	}

	/**
	 * Method that return default activation function name.
	 * @return default activation function name.
	 */
	public String getDefaultActvFunction() {
		return embedded.getDefaultActvFunction();
	}

	/**
	 * Method that indicates if the net has a default output function.
	 * @return true if net has default output function, false
	 * 	otherwise.
	 */
	public boolean hasDefaultOutFunction() {
		return embedded.hasDefaultOutFunction();
	}

	/**
	 * Method that permits to set a default output function for the
	 * net.
	 * @param func net default output function.
	 */
	public void setDefaultOutFunction(String func) {
		embedded.setDefaultOutFunction(func);
	}

	/**
	 * Method that return default output function name.
	 * @return default output function name.
	 */
	public String getDefaultOutFunction() {
		return embedded.getDefaultOutFunction();
	}

	/**
	 * Method that permits to add a node to the input layer of the net.
	 * @param node node reference.
	 */
	public void addInputNode(NetIFace.Node node) {
		embedded.addInputNode(node);
	}

	/**
	 * Method that returns net input layer size.
	 * @return net input layer size.
	 */
	public int getInputLayerSize() {
		return embedded.getInputLayerSize();
	}

	/**
	 * Method that returns net input layer's nodes iterator.
	 * @return net input layer's nodes iterator.
	 */
	public Iterator<NetIFace.Node> getInputLayerIterator() {
		return embedded.getInputLayerIterator();
	}

	/**
	 * Method that permits to add a node to the hidden layer of the net.
	 * @param node node reference.
	 */
	public void addHiddenNode(NetIFace.Node node) {
		embedded.addHiddenNode(node);
		fillBasket();
		scrambleGroups();
	}

	/**
	 * Method that returns net hidden layer size.
	 * @return net hidden layer size.
	 */
	public int getHiddenLayerSize() {
		return basket.size();
	}

	/**
	 * Method that returns net hidden layer's nodes iterator.
	 * @return net hidden layer's nodes iterator.
	 */
	public Iterator<NetIFace.Node> getHiddenLayerIterator() {
		return basket.iterator();
	}

	/**
	 * Method that permits to add a node to the output layer of the node.
	 * @param node node reference.
	 */
	public void addOutputNode(NetIFace.Node node) {
		embedded.addOutputNode(node);
	}

	/**
	 * Method that returns net output layer size.
	 * @return net output layer size.
	 */
	public int getOutputLayerSize() {
		return embedded.getOutputLayerSize();
	}

	/**
	 * Method that returns net output layer's nodes iterator.
	 * @return net output layer's nodes iterator.
	 */
	public Iterator<NetIFace.Node> getOutputLayerIterator() {
		return embedded.getOutputLayerIterator();
	}
}
