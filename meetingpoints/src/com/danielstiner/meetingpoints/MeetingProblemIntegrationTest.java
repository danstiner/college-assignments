package com.danielstiner.meetingpoints;

import static org.junit.Assert.assertEquals;

import java.io.ByteArrayInputStream;
import java.io.ByteArrayOutputStream;
import java.io.InputStream;
import java.io.OutputStream;
import java.util.Arrays;
import java.util.HashSet;
import java.util.Set;

import org.junit.Before;
import org.junit.Test;

import com.google.inject.Guice;
import com.google.inject.Injector;

public class MeetingProblemIntegrationTest {

	private Injector mInjector;

	@Before
	public void setUp() throws Exception {
		mInjector = Guice.createInjector(new MeetingModule());
	}

	@Test
	public void testLineSizeThree() {
		String input = "Map:\n" + "a b\n" + "b c\n" + "Avoid:\n" + "\n"
				+ "Normal:\n" + "a\n" + "Reversed:\n" + "c";
		Set<String> expected = makeExpected("a", "b", "c");

		Set<String> result = runLunchProblem(input);

		assertEquals(expected, result);
	}

	@Test
	public void testLineAvoidMiddle() {
		String input = "Map:\n" + "a b\n" + "b c\n" + "Avoid:\n" + "b\n"
				+ "Normal:\n" + "a\n" + "Reversed:\n" + "c";
		Set<String> expected = makeExpected();

		Set<String> result = runLunchProblem(input);

		assertEquals(expected, result);
	}

	@Test
	public void testLineAvoidStart() {
		String input = "Map:\n" + "a b\n" + "b c\n" + "Avoid:\n" + "a\n"
				+ "Normal:\n" + "a\n" + "Reversed:\n" + "c";
		Set<String> expected = makeExpected();

		Set<String> result = runLunchProblem(input);

		assertEquals(expected, result);
	}

	@Test
	public void testSquareAcross() {
		String input = "Map:\n" + "a c\n" + "a d\n" + "c d\n" + "c b\n"
				+ "a b\n" + "Avoid:\n" + "\n" + "Normal:\n" + "a\n"
				+ "Reversed:\n" + "c";
		Set<String> expected = new HashSet<String>(Arrays.asList("a", "c"));

		Set<String> result = runLunchProblem(input);

		assertEquals(expected, result);
	}

	@Test
	public void testSquareCorners() {
		String input = "Map:\n" + "a b\n" + "b c\n" + "d c\n" + "a d\n"
				+ "c a\n" + "Avoid:\n" + "\n" + "Normal:\n" + "d\n"
				+ "Reversed:\n" + "b";
		Set<String> expected = new HashSet<String>(Arrays.asList("a", "b", "c",
				"d"));

		Set<String> result = runLunchProblem(input);

		assertEquals(expected, result);

	}

	@Test
	public void testSquareThreeCorners() {
		String input = "Map:\n" + "d a\n" + "b a\n" + "c b\n" + "d c\n"
				+ "Avoid:\n" + "\n" + "Normal:\n" + "d\n" + "Reversed:\n" + "b";
		Set<String> expected = new HashSet<String>(Arrays.asList("c", "d", "b"));

		Set<String> result = runLunchProblem(input);

		assertEquals(expected, result);

	}

	@Test
	public void testComplex() {
		String input = "Map:\n" + "m a\n" + "k a\n" + "a c\n" + "b a\n"
				+ "f d\n" + "c d\n" + "f g\n" + "d g\n" + "h g\n" + "h j\n"
				+ "g j\n" + "g i\n" + "i h\n" + "j i\n" + "j d\n" + "Avoid:\n"
				+ "d\n" + "Normal:\n" + "m j f\n" + "Reversed:\n" + "k b h";
		Set<String> expected = new HashSet<String>(Arrays.asList("f", "g", "j",
				"h", "i"));

		Set<String> result = runLunchProblem(input);

		assertEquals(expected, result);

	}

	@Test
	public void testComplexMany() {
		String input = "Map:\n" + "m a\n" + "k a\n" + "a c\n" + "b a\n"
				+ "f d\n" + "c d\n" + "f g\n" + "d g\n" + "h g\n" + "h j\n"
				+ "g j\n" + "g i\n" + "i h\n" + "j i\n" + "j d\n" + "c e\n"
				+ "Avoid:\n" + "d\n" + "Normal:\n" + "m j f\n" + "Reversed:\n"
				+ "k b h e";
		Set<String> expected = new HashSet<String>(Arrays.asList("f", "g", "e",
				"c", "a", "m", "j", "h", "i"));

		Set<String> result = runLunchProblem(input);

		assertEquals(expected, result);

	}

	@Test
	public void testGiven() {
		String input = "Map:\n" + "a1 b1\n" + "a2 b1\n" + "a2 b2\n" + "a2 b3\n"
				+ "b1 c1\n" + "b2 c2\n" + "b2 c3\n" + "b3 c3\n" + "Avoid:\n"
				+ "b2\n" + "Peggy:\n" + "a2\n" + "Sam:\n" + "c2 c3";
		Set<String> expected = makeExpected("a2", "b3", "c3");

		Set<String> result = runLunchProblem(input);

		assertEquals(expected, result);

	}

	private Set<String> runLunchProblem(String input) {
		MeetingProblem problem = mInjector.getInstance(MeetingProblem.class);
		InputStream in = new ByteArrayInputStream(input.getBytes());
		OutputStream out = new ByteArrayOutputStream();

		problem.run(in, out);

		String result = out.toString().trim();

		if ("".equals(result))
			return new HashSet<String>();
		else
			return new HashSet<String>(Arrays.asList(result.split("\\r?\\n")));
	}

	private Set<String> makeExpected(String... expected) {
		return new HashSet<String>(Arrays.asList(expected));
	}

}
