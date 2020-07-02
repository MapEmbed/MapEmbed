CPFLAGS = -Wall -O2 -std=c++11

MapEmbed: main.cpp BOBHASH32.h MapEmbed.h
	g++ -o MapEmbed main.cpp $(CPFLAGS)

clean: 
	rm MapEmbed
