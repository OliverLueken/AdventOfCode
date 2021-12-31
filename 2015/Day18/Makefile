SHELL = /bin/sh
.SUFFIXES:
.SUFFIXES: .cpp .o

CPP = g++ -O3
CDEBUG = -g
CWARNINGS = -Wall -Wpedantic -Wextra -Wconversion
CPPFLAGS = -std=c++20 $(CDEBUG) $(CWARNINGS)

all: tasks.out

tasks.out: tasks.o lightShow.o
	$(CPP) -o tasks.out tasks.o lightShow.o $(CPPFLAGS)

.PHONY = clean
clean:
	rm *.o tasks.out