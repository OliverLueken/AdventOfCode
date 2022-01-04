SHELL = /bin/sh
.SUFFIXES:
.SUFFIXES: .cpp .o

CPP = g++
CDEBUG = -g
CWARNINGS = -Wall -Wpedantic -Wextra -Wconversion -Wshadow -Werror
CPPFLAGS = -std=c++20 -O3 $(CDEBUG) $(CWARNINGS)
CPPSANITIZER = -fsanitize=undefined,address
LIBS = -L/usr/lib/cryptopp/ -lcryptopp

SUBDIRS = 20*/Day*

all: $(SUBDIRS)


.SECONDEXPANSION:
$(SUBDIRS): $$(patsubst %.cpp,%.o,$$(wildcard $$@/*.cpp))
	$(CPP)  $(CPPFLAGS) $(CPPSANITIZER)    -o $@/tasks.out $^ $(LIBS)

.PHONY: clean
clean:
	rm */*/*.o */*/*.out