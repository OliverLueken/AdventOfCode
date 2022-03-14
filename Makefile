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

all: $(SUBDIRS)


.SECONDEXPANSION:
$(SUBDIRS): $$(patsubst %.cpp,%.o,$$(wildcard $$@/*.cpp))
	$(CPP)  $(CPPFLAGS)     -o $@/tasks.out $^ $(LIBS)

.PHONY: clean
clean:
	rm */*/*.o */*/*.out