package com.danielstiner.meetingpoints;

import java.util.Collection;

public interface IPerson {

	boolean getTravelReversed();

	Collection<INode> getStartingNodes();

	void addStartingNode(INode node);

}
