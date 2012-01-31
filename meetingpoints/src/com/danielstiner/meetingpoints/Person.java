package com.danielstiner.meetingpoints;

import java.util.Collection;
import java.util.LinkedList;

import com.google.inject.Inject;

public class Person implements IPerson {

	@Inject
	LinkedList<INode> startingNodes;

	@Override
	public void addStartingNode(INode node) {
		if (node != null)
			startingNodes.add(node);
	}

	@Override
	public Collection<INode> getStartingNodes() {
		return startingNodes;
	}

	@Override
	public boolean getTravelReversed() {
		return false;
	}

}
