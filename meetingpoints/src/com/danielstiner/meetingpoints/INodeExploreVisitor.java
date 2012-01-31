package com.danielstiner.meetingpoints;

import java.util.Collection;

import com.danielstiner.meetingpoints.INode.IVisitor;

public interface INodeExploreVisitor extends IVisitor {

	Collection<INode> explore(INode node, boolean travelReversed);

}
