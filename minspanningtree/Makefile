CXXFLAGS =	-O2 -g -Wall -fmessage-length=0

TARGET =	prims kruskals primslist

all:	prims kruskals

prims: prims.cpp PairingHeap.hpp common.hpp Graph.hpp
	$(CXX) -o prims prims.cpp $(CXXFLAGS)
	
primslist: prims.cpp PairingHeap.hpp common.hpp Graph.hpp
	$(CXX) -o primslist prims.cpp $(CXXFLAGS)
	
kruskals: kruskals.cpp UnionFind.hpp common.hpp Graph.hpp
	$(CXX) -o kruskals kruskals.cpp $(CXXFLAGS)

clean:
	rm -f prims kruskals primslist

	
.PHONY: all clean 