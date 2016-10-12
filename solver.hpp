/////////////////////////////////////////////////////////////////////////////////////
// Robert Wagner
// CISC 3410 Assignment #2
// 2016-10-02
// solver.hpp 
////////////////////////////////////////////////////////////////////////////////////
#ifndef __SUDOKU_SOLVER_HPP__
#define __SUDOKU_SOLVER_HPP__

#include <string>
#include <map>
#include <deque>
#include <vector>
#include <queue>
#include <sstream>
#include <iostream>
#include <climits>

namespace Sudoku {

    class InputEx { // invalid input validation exception handling
        private:
            char   index;
            char   value;
            string msg;
        public:
            InputEx(char _index, char _value, const string & _msg) : 
                index(_index), value(_value), msg(_msg) {}
            const char     getIndex()    const { return index; }
            const char     getValue()    const { return value; }
            const string & getMsg()      const { return msg;   }
    }; // class InputEx 

    class Solver {
        private:
            Board initial;
            Board solved;
            bool backtrack(Board & prev);
        public:
            Solver (const string &input); 
            bool backtrackSearch();
            bool isSolved() { return (solved.isSolved()); }
            Board solution();
    }; // class Solver
} // namespace Sudoku

#endif
