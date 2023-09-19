# IMPORTANT NOTE: Please create your own executable configuration for testing instead of overwriting an existing one!

EXENAME = main
OBJS = main.o graph.o utils.o loadfile.o

CXX = clang++
CXXFLAGS = $(CS225) -std=c++1y -stdlib=libc++ -c -g -Wall -Wextra -pedantic
LD = clang++
LDFLAGS = -std=c++1y -stdlib=libc++ -lc++abi -lm

# Custom Clang version enforcement Makefile rule:
ccred=$(shell echo -e "\033[0;31m")
ccyellow=$(shell echo -e "\033[0;33m")
ccend=$(shell echo -e "\033[0m")

IS_EWS=$(shell hostname | grep "ews.illinois.edu")
IS_CORRECT_CLANG=$(shell clang -v 2>&1 | grep "version 6")
ifneq ($(strip $(IS_EWS)),)
ifeq ($(strip $(IS_CORRECT_CLANG)),)
CLANG_VERSION_MSG = $(error $(ccred) On EWS, please run 'module load llvm/6.0.1' first when running CS225 assignments. $(ccend))
endif
else
CLANG_VERSION_MSG = $(warning $(ccyellow) Looks like you are not on EWS. Be sure to test on EWS before the deadline. $(ccend))
endif

all: $(EXENAME)

output_msg: ; $(CLANG_VERSION_MSG)

# Be sure to add output_msg as dependency target for your intro binary


$(EXENAME): output_msg $(OBJS) 
		$(LD) $(OBJS) $(LDFLAGS) -o $(EXENAME)

main.o: main.cpp 
		$(CXX) $(CXXFLAGS) main.cpp

graph.o: graph.h graph.cpp
		$(CXX) $(CXXFLAGS) graph.h graph.cpp

loadfile.o: loadfile.h loadfile.cpp
		$(CXX) $(CXXFLAGS) loadfile.h loadfile.cpp

utils.o: utils.cpp utils.h
		$(CXX) $(CXXFLAGS) utils.cpp

test-io.o: tests/test-io.cpp
		$(CXX) $(CXXFLAGS) tests/test-io.cpp

testio: output_msg test-io.o graph.o utils.o loadfile.o catchmain.o
		$(LD) test-io.o graph.o utils.o loadfile.o $(LDFLAGS) -o testio

test-graph.o: tests/test-graph.cpp
		$(CXX) $(CXXFLAGS) tests/test-graph.cpp

testgraph: output_msg test-graph.o graph.o utils.o catchmain.o
		$(LD) test-graph.o graph.o utils.o $(LDFLAGS) -o testgraph

test-markov.o: tests/test-markov.cpp
		$(CXX) $(CXXFLAGS) tests/test-markov.cpp

testmarkov: output_msg test-markov.o graph.o utils.o loadfile.o catchmain.o
		$(LD) test-markov.o graph.o utils.o loadfile.o $(LDFLAGS) -o testmarkov

catchmain.o: catch/catch.hpp catch/catchmain.cpp

clean:
	-rm -f *.o $(EXE_NAME) main testio testgraph testmarkov *.gch

.PHONY: output_msg
