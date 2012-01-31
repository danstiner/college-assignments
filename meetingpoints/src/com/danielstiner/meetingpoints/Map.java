package com.danielstiner.meetingpoints;

import java.util.HashMap;
import java.util.HashSet;

import com.google.inject.Inject;
import com.google.inject.Provider;

public class Map implements IMap {

	@Inject
	private Provider<INode> mNodeProvider;

	@Inject
	private HashSet<INode> mNodes;

	@Inject
	private HashMap<Object, INode> mNodeIndex;

	@Override
	public INode getNode(Object nodeId) {
		return mNodeIndex.get(nodeId);
	}

	@Override
	public INode createNode(Object id) {
		INode node = mNodeProvider.get();
		node.setId(id);

		mNodes.add(node);
		mNodeIndex.put(id, node);

		return node;
	}

}
