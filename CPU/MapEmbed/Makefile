CPFLAGS = -g -Wall -O3 -std=c++17

MapEmbed: main.cpp murmur3.h MapEmbed.h
	g++ -mavx2 main.cpp -o MapEmbed $(CPFLAGS)

clean: 
	rm MapEmbed
