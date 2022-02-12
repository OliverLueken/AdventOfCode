SHELL = /bin/sh
.SUFFIXES:
.SUFFIXES: .cpp .o

CPP = g++
CPPDEBUG = -g
CPPWARNINGS = -Wall -Wpedantic -Wextra -Wconversion
CPPSANITIZER = -fsanitize=undefined,address
CPPFLAGS = -std=c++20 -O3 $(CPPWARNINGS)

all: tasks.out

tasks.out: tasks.cpp Picture.hpp Picture.cpp Tile.hpp Tile.cpp
	$(CPP) tasks.cpp Picture.cpp Tile.cpp -o tasks.out $(CPPFLAGS) $(CPPWARNINGS) $(CPPSANITIZER)

.PHONY = clean
clean:
	rm *.o tasks.out