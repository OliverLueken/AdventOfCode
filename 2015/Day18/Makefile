CC = g++
CFLAGS = -std=c++20 -Wall -Wpedantic -Wextra -Wconversion -g -O3


all: tasks.out lightShow.o

tasks.out: tasks.o lightShow.o
	$(CC) $(CFLAGS) -o tasks.out tasks.o lightShow.o

tasks.o: tasks.cpp lightShow.h
	$(CC) $(CFLAGS) -c tasks.cpp

lightShow.o: lightShow.cpp lightShow.h
	$(CC) $(CFLAGS) -c lightShow.cpp


clean:
	rm *.o