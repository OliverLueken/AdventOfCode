
CPP = g++
CPPFLAGS = -std=c++20 -Wall -Wpedantic -Wextra -Wconversion -O3

all: tasks.out tasks.o battleground.o enemy.o magician.o spell.o effect.o

tasks.out: tasks.o battleground.o enemy.o magician.o spell.o effect.o
	$(CPP) $(CPPFLAGS) -o tasks.out tasks.o battleground.o enemy.o magician.o spell.o effect.o

clean:
	rm *.o