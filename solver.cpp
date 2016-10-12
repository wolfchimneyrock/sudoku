/////////////////////////////////////////////////////////////////////////////////////
// Robert Wagner
// CISC 3410 Assignment #2
// 2016-10-02
// solver.cpp 
////////////////////////////////////////////////////////////////////////////////////
#include "board.hpp"
#include "solver.hpp"

namespace Sudoku {

    Solver::Solver(const string &input) {
        char value;
        int index = 0;
        std::stringstream SS(input);
        while (SS >> value && index < SIZE)  {
            if (value == '.') { index++; continue; }
            if (value < '0' || value > '9')   
               throw InputEx(index + 1, value, "out of range [0..9]");
            if (value != '0' && !initial(index, value - '0')) 
                throw InputEx(index + 1, value, "duplicate values");
            index++;
        }
        DataType dist = 0;
        while (initial.distance() != dist) {
            dist = initial.distance();
            initial.trimDomains();
            initial.clearSolved();
        }
        if (initial.isSolved()) solved = initial;
        initial.buildAllConstraints();
    } // Solver()

    bool Solver::backtrack(Board & prev) {
        if (prev.isSolved()) { solved = Board(prev); return true; }
        bool result = false;
        DataType var = prev.chooseUnassigned();
        ValueVec values = prev.getOrderedDomain(var);
        Board csp;
        for (auto const &n : values) {
            csp = prev;
            if (!csp(var, n))      continue;
            csp.buildConstraints(var);
            if (!csp.performAC3()) continue;
            if (!csp.isValid())    continue;
            csp.trimDomains();
            csp.clearSolved();
            if (csp.isValid()) result = backtrack(csp);
            if (result) return result;
        }
        return result;
    }

    bool Solver::backtrackSearch() { return backtrack(initial); }
    Board Solver::solution() { if (solved.isSolved()) return solved; else return initial; }
} // namespace Sudoku
        
