/////////////////////////////////////////////////////////////////////////////////////
// Robert Wagner
// CISC 3410 Assignment #2
// 2016-10-02
// board.cpp 
////////////////////////////////////////////////////////////////////////////////////
#include "board.hpp"

namespace Sudoku {

    std::map<std::string, int> UserFlag;
    
    Board::Board() {
            std::fill_n(values,  SIZE, 0);
            std::fill_n(degrees, SIZE, 0);
            for (auto i = 0; i < SIZE; i++) domains[i] = ONES; 
            unsolved = SolvedSet(0).flip();
    } // Board plain constructor

    Board::Board(const Board & other) : unsolved(other.unsolved) { 
        for (auto i = 0; i < SIZE; i++) {
            domains[i] = other.domains[i];
            degrees[i] = other.degrees[i];
            values[i]  = other.values[i];
        }
    } // Board copy constructor

    ValueVec Neighbors::getNeighbors(const DataType x) {
        DataType row = RowOf[x];
        DataType col = ColOf[x];
        DataType j;
        ValueVec temp; // up to 20 neighbors in 9x9 sudoku
        temp.reserve(20);
        for (auto i = 0; i < DIM; i++) {
            j = i + row * DIM;
            if (j != x) temp.push_back(j);
            j = col + i * DIM; 
            if (j != x) temp.push_back(j);
        }

        // after adding row & column neighbors, there are only 4 more in the block to add
        DataType r1 = 0, r2 = 0, c1 = 0, c2 = 0;
        switch (col % BLOCKSIZE) {
            case 0: c1 = col + 1; c2 = col + 2; break;
            case 1: c1 = col - 1; c2 = col + 1; break;
            case 2: c1 = col - 2; c2 = col - 1; break;
        }
        switch (row % BLOCKSIZE) {
            case 0: r1 = row + 1; r2 = row + 2; break;
            case 1: r1 = row - 1; r2 = row + 1; break;
            case 2: r1 = row - 2; r2 = row - 1; break;
        }
        temp.push_back(c1 + DIM * r1);
        temp.push_back(c1 + DIM * r2);
        temp.push_back(c2 + DIM * r1);
        temp.push_back(c2 + DIM * r2);
        return temp;
    } // Board::neighbors()

    void Board::buildAllConstraints() {
        constraints.clear();
        std::fill_n(degrees, SIZE, 0);
        for (auto from = 0; from < SIZE; from++) {
            if (unsolved[from])
                for (auto const &to : neighbors(from)) {
                    constraints.push_back(Constraint(from, to));
                    degrees[from]++;
                }
        }
    } // Board::buildAllConstraints()
    
    void Board::buildConstraints(const DataType from) {
        constraints.clear();
        for (auto const &to : neighbors(from)) {
            constraints.push_back(Constraint(from, to));
        }
    } // Board::buildConstraints(from)
    
    bool Board::clearSolved() {
        bool changed = false;
        for (auto i = 0; i < SIZE; i++) {
            if (unsolved[i] && domains[i].count() == 1) {
                unsolved[i] = false;
                DataType j = 0;
                while ((domains[i])[j] == false) j++;
                values[i] = j + 1;
                changed = true;
            }
        }
        return changed;
    } // Board::clearSolved()

    bool Board::trimDomains() {
        bool changed = false;
        for (auto i = 0; i < SIZE; i++) 
            if (!unsolved[i]) 
                for (auto const &j : neighbors(i)) {
                    (domains[j])[values[i]-1] = false;
                    changed = true;
                }
        return changed;
    } // Board::trimDomains()

    bool Board::reviseDomain(const DataType x, const DataType y) {
        bool revised = false;
        DomainSet val;
        for (auto i = 0; i < DIM; i++) 
            if ((domains[x])[i]) {
                val = domains[y];
                val[i] = false;
                if (val.none()) {
                    revised = true;
                    (domains[x])[i] = false;
                }
            }
        return revised; 
    } // Board::reviseDomain()

    DataType Board::chooseUnassigned() const {
        // perform a radix sort of unassigned cells
        ValueVec buckets[DIM + 1];
        for (auto i = 0; i < SIZE; i++)
            if (values[i] == 0) {
                buckets[domains[i].count()].push_back(i);
            }
        DataType minRV = 0;
        while (minRV < DIM && buckets[minRV].size() == 0) minRV++;
        DataType maxDegrees = -1;
        DataType maxCell    = 0;
        for (auto const &m : buckets[minRV]) {
            if (degrees[m] > maxDegrees) {
                maxCell = m;
                maxDegrees = degrees[m];
            }
        }
        return maxCell;
    } // Board::chooseUnassigned()


    bool  Board::performAC3() {
        while (!constraints.empty()) {
            Constraint c = constraints.front(); 
            constraints.pop_front();
            degrees[c.second]--;
            if (reviseDomain(c.first, c.second)) {
                if ((domains[c.first]).none()) return false;
                for (auto const &i : neighbors(c.first)) {
                    constraints.push_back(Constraint(i, c.first));
                    degrees[i]++;
                }
            }
        }
        return true;
    } // Board::performAC3()

    const ValueVec Board::getOrderedDomain(const DataType index) const {
        ValueVec temp;
        temp.reserve(9);
        for (auto i = 0; i < DIM; i++)
            if ((domains[index])[i]) 
                temp.push_back(i + 1);
        return temp;
    } // Board::getOrderedDomain()

    bool Board::operator()(const DataType index, const DataType val) {
        if (val == 0) {
            domains[index] = ONES; 
            unsolved[index] = true;
            return true;
        }
        if((domains[index])[val - 1] == false) return false;
        domains[index] &= SINGLES[val - 1]; 
        for (auto const &n : neighbors(index)) {
            (domains[n])[val - 1]=false;
            if (domains[n].none()) return false;
        }
        values[index] = val;
        unsolved[index] = false;
        return true;
    } // Board::operator()()

    std::ostream& pretty(std::ostream& out) {
        if (UserFlag.find("BoardFormat") == UserFlag.end())
            UserFlag["BoardFormat"] = out.xalloc();
        out.iword(UserFlag["BoardFormat"]) = PRETTY;
        return (out);
    }

    std::ostream& plain(std::ostream& out) {
        if (UserFlag.find("BoardFormat") == UserFlag.end())
            UserFlag["BoardFormat"] = out.xalloc();
        out.iword(UserFlag["BoardFormat"]) = PLAIN;
        return (out);
    }

    std::ostream& count(std::ostream& out) {
        if (UserFlag.find("BoardCount") == UserFlag.end())
            UserFlag["BoardCount"] = out.xalloc();
        out.iword(UserFlag["BoardCount"]) = COUNT;
        return (out);
    }

    std::ostream& operator<<(std::ostream& out, const Board &b) {
        if (out.iword(UserFlag["BoardFormat"]) == PRETTY) {
            for (auto i = 0; i < SIZE; i++) {
                if (i % (DIM * BLOCKSIZE) == 0) 
                   out << fmtHLine; 
                if (i % DIM == 0) out << "|";
                if (0 == b.values[i]) out << " .";
                else out << " " << (int)(b.values[i]);
                if ((i+1) % BLOCKSIZE == 0) out << " |";
                if ((i+1) % DIM == 0) out << std::endl; 
            }
            out << fmtHLine; 
        } else {
            for (auto i = 0; i < SIZE; i++) out << (int)(b.values[i]);
            out << std::endl;
        }
        return (out);
    } // operator<< 

} // namespace Sudoku
