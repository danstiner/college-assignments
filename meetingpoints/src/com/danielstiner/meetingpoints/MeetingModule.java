package com.danielstiner.meetingpoints;

import com.google.inject.AbstractModule;
import com.google.inject.name.Names;

public class MeetingModule extends AbstractModule {

	public static final String PERSON_NORMAL = "Normal";
	public static final String PERSON_REVERSED = "Reversed";

	@Override
	protected void configure() {
		bind(INode.class).to(Node.class);
		bind(IMap.class).to(Map.class);
		bind(IProblem.class).to(MeetingProblem.class);
		bind(INodeExploreVisitor.class).to(NodeExploreVisitor.class);
		bind(IPerson.class).annotatedWith(Names.named(PERSON_NORMAL)).to(
				Person.class);
		bind(IPerson.class).annotatedWith(Names.named(PERSON_REVERSED)).to(
				PersonReversed.class);
	}

}
