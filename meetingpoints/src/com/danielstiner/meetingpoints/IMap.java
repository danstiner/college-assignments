package com.danielstiner.meetingpoints;


public interface IMap {

	INode getNode(Object nodeId);

	INode createNode(Object id);

}
