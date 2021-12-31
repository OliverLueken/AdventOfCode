SHELL = /bin/sh
.SUFFIXES:
.SUFFIXES: .cpp .o

CPP = g++ -O3
CDEBUG = -g
CWARNINGS = -Wall -Wpedantic -Wextra -Wconversion
CPPFLAGS = -std=c++20 $(CDEBUG) $(CWARNINGS)

all: tasks.out

objects = tasks.o battleground.o effect.o enemy.o magician.o spell.o

tasks.out: $(objects)
	$(CPP) -o tasks.out $(objects) $(CPPFLAGS)

.PHONY = clean
clean:
	rm *.o tasks.out