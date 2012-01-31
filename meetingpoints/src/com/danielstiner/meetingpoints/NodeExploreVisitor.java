package com.danielstiner.meetingpoints;

import java.util.Collection;
import java.util.HashSet;

public class NodeExploreVisitor implements INodeExploreVisitor {

	private boolean mTravelReversed = false;

	private HashSet<INode> mExplored;

	@Override
	public boolean visit(Node junction) {
		return mExplored.add(junction);
	}

	@Override
	public boolean travelReversed() {
		return mTravelReversed;
	}

	@Override
	public Collection<INode> explore(INode node, boolean travelReversed) {

		mExplored = new HashSet<INode>();

		mTravelReversed = travelReversed;

		node.accept(this);

		return mExplored;
	}

}
