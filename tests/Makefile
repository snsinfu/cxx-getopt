# Copyright snsinfu 2018.
# Distributed under the Boost Software License, Version 1.0. (See accompanying
# file LICENSE.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

INCLUDES = \
  -isystem Catch1

OPTFLAGS = \
  -O2

DBGFLAGS = \
  -g       \
  -fsanitize=address

CXXFLAGS =          \
  -std=c++03        \
  -pedantic         \
  -Wall             \
  -Wextra           \
  -Wshadow          \
  -Wconversion      \
  -Wsign-conversion \
  $(INCLUDES)       \
  $(OPTFLAGS)       \
  $(DBGFLAGS)

OBJECTS = \
  main.o  \
  specs.o \
  examples.o

ARTIFACTS =  \
  $(OBJECTS) \
  run

.SUFFIXES: .cc
.PHONY: test clean

test: run
	./run

clean:
	rm -f $(ARTIFACTS)

run: $(OBJECTS) $(HEADERS)
	$(CXX) $(CXXFLAGS) $(LDFLAGS) -o $@ $(OBJECTS)

.cc.o:
	$(CXX) $(CXXFLAGS) -c -o $@ $<

main.o: main.cc Catch1/catch.hpp
specs.o: specs.cc Catch1/catch.hpp ../getopt.hpp
examples.o: examples.cc Catch1/catch.hpp ../getopt.hpp
