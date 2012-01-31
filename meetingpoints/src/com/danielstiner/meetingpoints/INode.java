package com.danielstiner.meetingpoints;

public interface INode {

	public interface IVisitor {

		/**
		 * Applies vistor to a node of type Junction
		 * 
		 * @param junction
		 * @return True to continue traversing the rivers connected to this
		 *         junction
		 */
		boolean visit(Node junction);

		/**
		 * When true, the normal direction of traversal in the directed graph is
		 * reversed while visiting
		 * 
		 * @return
		 */
		boolean travelReversed();

	}

	/**
	 * Calls visit on the visitor for this node. And if that returns true, will
	 * recursively do the same for all nodes connected by outgoing links
	 * normally, unless travelReversed on the visitor is true, then incoming
	 * links are recursed
	 * 
	 * @param visitor
	 */
	void accept(IVisitor visitor);

	void connect(INode to);

	
	void setAvoid(boolean b);

	void setId(Object id);

	void reverseConnect(INode from);

}
