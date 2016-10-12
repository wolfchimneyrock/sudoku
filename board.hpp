/////////////////////////////////////////////////////////////////////////////////////
// Robert Wagner
// CISC 3410 Assignment #2
// 2016-10-02
// board.hpp 
/////////////////////////////////////////////////////////////////////////////////////
#ifndef __SUDOKU_BOARD_HPP__
#define __SUDOKU_BOARD_HPP__
#include <string>
#include <map>
#include <deque>
#include <vector>
#include <sstream>
#include <iostream>
#include <iomanip>
#include <climits>
#include <algorithm>
#include <bitset>

namespace Sudoku {
using std::string;
using std::vector;
using std::deque;
using std::map;
using std::bitset;
using std::pair;

const string fmtHLine  =   "+-------+-------+-------+\n"; 
const int    BLOCKSIZE =   3;
const int    DIM       =   BLOCKSIZE * BLOCKSIZE;
const int    SIZE      =   DIM * DIM;


using DataType               = char; // generates warnings but is faster than int
using Constraint             = pair<DataType, DataType>;
using DomainSet              = bitset<DIM>;
using SolvedSet              = bitset<SIZE>;
using ValueVec               = vector<DataType>;
const DomainSet ONES         = DomainSet(0).flip();
const DomainSet SINGLES[DIM] = {  DomainSet(1), 
                                  DomainSet(1 << 1),
                                  DomainSet(1 << 2),
                                  DomainSet(1 << 3),
                                  DomainSet(1 << 4),
                                  DomainSet(1 << 5),
                                  DomainSet(1 << 6),
                                  DomainSet(1 << 7),
                                  DomainSet(1 << 8) };
class Neighbors {
    ValueVec n[SIZE];
    ValueVec getNeighbors(const DataType x);
    public:
    Neighbors() {
        for (auto i = 0; i < SIZE; i++)
            n[i] = getNeighbors(i);
    }
    const ValueVec & operator()(const DataType x) const { return n[x]; }
};

static Neighbors neighbors;

class Board {
    private:
        DomainSet          domains[SIZE];
        deque<Constraint>  constraints;
        SolvedSet          unsolved;
        DataType           values[SIZE];
        DataType           degrees[SIZE]; 
        bool               reviseDomain(const DataType x, const DataType y); 
    public:

        DataType           chooseUnassigned() const;
        bool               operator()(const DataType index, const DataType val); 
        void               buildAllConstraints(); 
        void               buildConstraints(const DataType from); 
        bool               clearSolved();
        bool               trimDomains();

        const ValueVec     getOrderedDomain(const DataType index) const;
        
        Board();
        Board(const Board & other);
        ~Board() {}

        DataType  distance() const { return unsolved.count(); }
        bool      isSolved() const { return unsolved.none();  }
        bool      performAC3();        
        bool      isValid() const { 
           for (auto const &d : domains) 
              if (d.none()) return false;
           return true;
        }

        friend std::ostream& operator<<(std::ostream& out, const Board &b);
        

    }; // class Board

    // ostream formatting tokens to display game board
    std::ostream& pretty(std::ostream& out);
    std::ostream& plain (std::ostream& out);
    std::ostream& count (std::ostream& out);

    const int PRETTY = 0x100;
    const int PLAIN  = 0x200;
    const int TIME   = 0x300;
    const int COUNT  = 0x400;

    const DataType BlkOf[81]  = { 0,0,0,1,1,1,2,2,2,
                                  0,0,0,1,1,1,2,2,2,
                                  0,0,0,1,1,1,2,2,2,
                                  3,3,3,4,4,4,5,5,5,
                                  3,3,3,4,4,4,5,5,5,
                                  3,3,3,4,4,4,5,5,5,
                                  6,6,6,7,7,7,8,8,8,
                                  6,6,6,7,7,7,8,8,8,
                                  6,6,6,7,7,7,8,8,8 };

    const DataType RowOf[81]  = { 0,0,0,0,0,0,0,0,0,
                                  1,1,1,1,1,1,1,1,1,
                                  2,2,2,2,2,2,2,2,2,
                                  3,3,3,3,3,3,3,3,3,
                                  4,4,4,4,4,4,4,4,4,
                                  5,5,5,5,5,5,5,5,5,
                                  6,6,6,6,6,6,6,6,6,
                                  7,7,7,7,7,7,7,7,7,
                                  8,8,8,8,8,8,8,8,8 };

    const DataType ColOf[81]  = { 0,1,2,3,4,5,6,7,8,
                                  0,1,2,3,4,5,6,7,8,
                                  0,1,2,3,4,5,6,7,8,
                                  0,1,2,3,4,5,6,7,8,
                                  0,1,2,3,4,5,6,7,8,
                                  0,1,2,3,4,5,6,7,8,
                                  0,1,2,3,4,5,6,7,8,
                                  0,1,2,3,4,5,6,7,8,
                                  0,1,2,3,4,5,6,7,8 };

}  // namespace Sudoku
        
#endif
