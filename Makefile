SHELL = /bin/sh
.SUFFIXES:
.SUFFIXES: .cpp .o

CPP = g++
CPPDEBUG = -g
CPPWARNINGS = -Wall -Wpedantic -Wextra -Wconversion -Wshadow -Werror
CPPSANITIZER = -fsanitize=undefined,address
CPPFLAGS = -std=c++20 -O3 $(CPPWARNINGS) $(CPPDEBUG) $(CPPSANITIZER)
LIBS = -L/usr/lib/cryptopp/ -lcryptopp

SUBDIRS = 20*/Day*
COMPUTERDIRS = 2015/Day06 2015/Day23 2017/Day05 2017/Day08
#2016/Day01 2016/Day08 2016/Day10 2016/Day12 2016/Day21 2016/Day23 2016/Day25 2020/Day08 2020/Day12 2021/Day02 2021/Day24

all: $(SUBDIRS)

computer: $(COMPUTERDIRS)

.SECONDEXPANSION:
$(SUBDIRS): $$(patsubst %.cpp,%.o,$$(wildcard $$@/*.cpp))
	$(CPP)  $(CPPFLAGS)     -o $@/tasks.out $^ $(LIBS)


.PHONY: clean
clean:
	rm */*/*.o */*/*.out