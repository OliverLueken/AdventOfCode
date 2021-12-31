SHELL = /bin/sh
.SUFFIXES:
.SUFFIXES: .cpp .o

CPP = g++ -O3
CDEBUG = -g
CWARNINGS = -Wall -Wpedantic -Wextra -Wconversion
CPPFLAGS = -std=c++20 $(CDEBUG) $(CWARNINGS) -L/usr/lib/cryptopp/ -lcryptopp

all: tasks.out

tasks.out: tasks.o
	$(CPP) -o tasks.out tasks.o $(CPPFLAGS)

.PHONY = clean
clean:
	rm *.o tasks.out