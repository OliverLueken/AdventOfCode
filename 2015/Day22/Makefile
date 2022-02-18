SHELL = /bin/sh
.SUFFIXES:
.SUFFIXES: .cpp .o

CPP = g++
CPPDEBUG = -g
CPPWARNINGS = -Wall -Wpedantic -Wextra -Wconversion -Wshadow -Werror
CPPSANITIZER = -fsanitize=undefined,address
CPPFLAGS = -std=c++20 -O3 $(CPPDEBUG) $(CPPWARNINGS) $(CPPSANITIZER)

all: tasks.out

objects = tasks.o battleground.o effect.o enemy.o magician.o spell.o

tasks.out: $(objects)
	$(CPP) -o tasks.out $(objects) $(CPPFLAGS)

.PHONY = clean
clean:
	rm *.o tasks.out