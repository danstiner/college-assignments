package com.danielstiner.meetingpoints;

import java.util.Collection;
import java.util.HashSet;

import com.google.inject.Inject;

public class Node implements INode {

	private boolean mAvoid;
	private Object mId = "defaultNodeId";
	
	@Inject
	private HashSet<INode> mOutgoing;
	
	@Inject
	private HashSet<INode> mIncoming;

	@Override
	public void accept(IVisitor visitor) {
		if (mAvoid)
			return;

		// Stop if visit fails
		if (!visitor.visit(this))
			return;

		Collection<INode> neighborJunctions;
		if (visitor.travelReversed()) {
			neighborJunctions = this.getInbound();
		} else {
			neighborJunctions = this.getOutbound();
		}

		for (INode neighbor : neighborJunctions) {
			neighbor.accept(visitor);
		}
	}

	private Collection<INode> getInbound() {
		return mIncoming;
	}

	private Collection<INode> getOutbound() {
		return mOutgoing;
	}

	@Override
	public int hashCode() {
		if (mId != null)
			return mId.hashCode();

		return super.hashCode();
	}

	@Override
	public boolean equals(Object obj) {

		if (obj == this)
			return true;
		
		if (!(obj instanceof Node))
			return false;

		Node other = (Node)obj;

		if (!other.mId.equals(this.mId))
			return false;

		return true;
	}
	
	@Override
	public String toString() {
		return mId.toString();
	}

	@Override
	public void connect(INode to) {	
		if(this.mOutgoing.add(to))
			to.reverseConnect(this);
	}
	
	@Override
	public void reverseConnect(INode from) {
		if(this.mIncoming.add(from))
			from.connect(this);
	}

	@Override
	public void setAvoid(boolean b) {
		mAvoid = b;
	}

	@Override
	public void setId(Object id) {
		this.mId = id;
	}

}
