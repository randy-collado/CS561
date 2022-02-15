CFLAGS = -g -std=c++11 -o

all: main

main: main.cpp TPHQueryGenerator.cpp
		g++ $(CFLAGS) $@ $^

clear:
	rm main
