CXX      = g++
OBJS     = board.o solver.o sudoku.o
#DEBUG    = -g -pg
#WARN     = -Wall
CXXFLAGS = -std=c++11 -O3 

CFLAGS = $(CXXFLAGS) $(WARN) $(DEBUG) -c
LFLAGS = $(CXXFLAGS) $(WARN) $(DEBUG)

sudoku: $(OBJS)
	$(CXX) $(LFLAGS) $(OBJS) -o sudoku

board.o: board.cpp board.hpp
	$(CXX) $(CFLAGS) board.cpp

solver.o: solver.cpp solver.hpp board.hpp
	$(CXX) $(CFLAGS) solver.cpp

sudoku.o: sudoku.cpp solver.hpp board.hpp
	$(CXX) $(CFLAGS) sudoku.cpp

clean: 
	rm *.o sudoku

all: clean sudoku


