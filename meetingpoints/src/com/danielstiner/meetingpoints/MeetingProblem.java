package com.danielstiner.meetingpoints;

import java.io.InputStream;
import java.io.OutputStream;
import java.io.PrintStream;
import java.util.Collection;
import java.util.HashSet;
import java.util.LinkedList;
import java.util.Scanner;
import java.util.regex.Pattern;

import com.google.inject.Inject;
import com.google.inject.Provider;
import com.google.inject.name.Named;

public class MeetingProblem implements IProblem {

	private static final String NODE_SEPARATOR = " ";

	@Inject
	private Provider<IMap> mMapProvider;

	@Inject
	@Named(MeetingModule.PERSON_NORMAL)
	private Provider<IPerson> mNormalPersonProvider;

	@Inject
	@Named(MeetingModule.PERSON_REVERSED)
	private Provider<IPerson> mReversedPersonProvider;

	@Inject
	private Provider<NodeExploreVisitor> mNodeExplorerProvider;

	private final Pattern mMapConnectionPattern = Pattern
			.compile("[^ ]+ [^ ]+");

	@Override
	public void run(InputStream in, OutputStream out) {

		// Print out nodes
		printNodes(findMeetingNodes(in), out);
	}

	Collection<INode> findMeetingNodes(InputStream in) {
		Scanner inScanner = new Scanner(in);

		// Read in network data
		IMap network = parseMap(inScanner);

		// Read in first person
		IPerson normalPerson = mNormalPersonProvider.get();
		parsePerson(normalPerson, inScanner, network);

		// Read in second person
		IPerson reversedPerson = mReversedPersonProvider.get();
		parsePerson(reversedPerson, inScanner, network);

		// Compute nodes we are interested in
		return findMeetingNodes(normalPerson, reversedPerson);
	}

	private void parsePerson(IPerson person, Scanner in, IMap network) {

		// Read first line, should say "Person Name:"
		in.nextLine();

		// Read second line, delimited set of starting locations
		for (String startNodeId : in.nextLine().split(NODE_SEPARATOR)) {
			person.addStartingNode(network.getNode(startNodeId));
		}
	}

	private IMap parseMap(Scanner in) {
		IMap network = mMapProvider.get();
		String line;

		// Read first line, should say "Map:"
		in.nextLine();

		// Start reading input pairs

		while ((line = in.findInLine(mMapConnectionPattern)) != null) {

			Object fromId = line.split(NODE_SEPARATOR)[0];
			Object toId = line.split(NODE_SEPARATOR)[1];

			INode from;
			INode to;

			if ((from = network.getNode(fromId)) == null)
				from = network.createNode(fromId);

			if ((to = network.getNode(toId)) == null)
				to = network.createNode(toId);

			from.connect(to);

			// Move to the next line
			in.nextLine();
		}

		// Read "Avoid:" line
		in.nextLine();

		// Read all avoid nodes from a single line
		line = in.nextLine();
		for (String nodeId : line.split(NODE_SEPARATOR)) {
			INode node = network.getNode(nodeId);
			if (node != null)
				node.setAvoid(true);
		}

		// Done
		return network;

	}

	/**
	 * Finds possible meeting nodes for the two people
	 * 
	 * Looks at each possible start node for p1, and explores their reachable
	 * nodes. For each exploration, each of the start nodes for p2 is also
	 * explored. Finally, for each combination of p1 and p2 explorations, the
	 * intersection of their reachable nodes is computed.
	 * 
	 * That intersection is the possible meeting nodes for the two people.
	 * 
	 * @param p1
	 * @param p2
	 * @return
	 */
	private Collection<INode> findMeetingNodes(IPerson p1, IPerson p2) {
		Collection<INode> explored1 = new HashSet<INode>();
		Collection<INode> explored2 = new HashSet<INode>();

		for (INode node : p1.getStartingNodes())
			explored1.addAll(mNodeExplorerProvider.get().explore(
					node, p1.getTravelReversed()));
		
		for (INode node : p2.getStartingNodes())
			explored2.addAll(mNodeExplorerProvider.get().explore(
					node, p2.getTravelReversed()));
		
		// Find possible meeting points by intersection of explorations
		explored2.retainAll(explored1);

		return explored2;
	}

	private static void printNodes(Collection<INode> nodes, OutputStream to) {
		PrintStream ps = new PrintStream(to);
		for (INode node : nodes) {
			ps.println(node);
		}
	}
}
