CPP = g++
CPPFLAGS = -std=c++20 -Wall -Wpedantic -Wextra -Wconversion -O3


all: tasks.out tasks.o lightShow.o

tasks.out: tasks.o lightShow.o
	$(CPP)  -o tasks.out tasks.o lightShow.o $(CPPFLAGS)

.PHONY = clean
clean:
	rm *.o tasks.out