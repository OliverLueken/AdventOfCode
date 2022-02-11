SHELL = /bin/sh
.SUFFIXES:
.SUFFIXES: .cpp .o

CPP = g++
CPPDEBUG = -g
CPPWARNINGS = -Wall -Wpedantic -Wextra -Wconversion
CPPSANITIZER = -fsanitize=undefined,address
CPPFLAGS = -std=c++20 -O3 $(CPPWARNINGS)

all: tasks.out tasks.o Tile.o Picture.o

tasks.out: tasks.o Tile.o Picture.o
	$(CPP) -o tasks.out tasks.o Tile.o Picture.o $(CPPFLAGS) $(CPPWARNINGS) $(CPPSANITIZER)

tasks.o: tasks.cpp Tile.hpp Picture.hpp
Picture.o: Picture.hpp Picture.cpp Tile.hpp
Tile.o: Tile.hpp Tile.cpp

.PHONY = clean
clean:
	rm *.o tasks.out