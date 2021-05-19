CPFLAGS = -g -Wall -O3 -std=c++17 

DynamicPerfectHashing: dph.cpp murmur3.h DynamicPerfectHashing.h
	g++ -mavx2 dph.cpp -o dph $(CPFLAGS)

clean: 
	rm dph
