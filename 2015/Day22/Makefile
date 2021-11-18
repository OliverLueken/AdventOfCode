
CC = g++
CFLAGS = -std=c++20 -Wall -Wpedantic -Wextra -Wconversion -g -O3

all: a.out tasks.o battleground.o enemy.o magician.o spell.o effect.o

a.out: tasks.o battleground.o enemy.o magician.o spell.o effect.o
	$(CC) $(CFLAGS) -o a.out tasks.o battleground.o enemy.o magician.o spell.o effect.o

tasks.o: tasks.cpp battleground.h enemy.h magician.h spell.h effect.h ../../readFile.h ../../utilities.h
	$(CC) $(CFLAGS) -c tasks.cpp

battleground.o: battleground.cpp battleground.h enemy.h magician.h spell.h effect.h
	$(CC) $(CFLAGS) -c battleground.cpp

enemy.o: enemy.cpp battleground.h enemy.h magician.h spell.h effect.h ../../utilities.h
	$(CC) $(CFLAGS) -c enemy.cpp

magician.o: magician.cpp battleground.h enemy.h magician.h spell.h effect.h
	$(CC) $(CFLAGS) -c magician.cpp

spell.o: spell.cpp battleground.h enemy.h magician.h spell.h effect.h
	$(CC) $(CFLAGS) -c spell.cpp

effect.o: effect.cpp battleground.h enemy.h magician.h effect.h
	$(CC) $(CFLAGS) -c effect.cpp

clean:
	rm *.o