package com.danielstiner.meetingpoints;

import com.google.inject.Guice;

/**
 * @author Daniel Stiner <stiner@iastate.edu>
 *
 */
public class MeetingPointsProgram {

	public static void main(String[] args) {
		// Pass off work to an actual instance of this class
		// that is dependency injected
		Guice.createInjector(new MeetingModule()).getInstance(MeetingProblem.class)
				.run(System.in, System.out);
	}

}
