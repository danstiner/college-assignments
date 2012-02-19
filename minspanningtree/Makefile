CXXFLAGS =	-O2 -g -Wall -fmessage-length=0

OBJS =		315x-project1.o PairingHeap.o

LIBS =

#TARGET =	project1

project1: project1.cpp PairingHeap.hpp
	$(CXX) -o project1 project1.cpp $(CXXFLAGS)

all:	project1

clean:
	rm -f project1 project1.o PairingHeap.o
