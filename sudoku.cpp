/////////////////////////////////////////////////////////////////////////////////////
// Robert Wagner
// CISC 3410 Assignment #2
// 2016-10-02
// sudoku.cpp 
////////////////////////////////////////////////////////////////////////////////////
#include <iostream>
#include <fstream>
#include <string>
#include "board.hpp"
#include "solver.hpp"

using namespace Sudoku;
using std::to_string;
std::ostream& output = std::cout;

std::string fmtPrompt(int lineNo) {
    return "[" + to_string(lineNo) + "] >> ";
}

std::string fmtTitle(int lineNo, std::string msg) {
    return "[" + to_string(lineNo) + "] " + msg + "\n";
}
std::string fmtError(int lineNo, int pos, std::string msg, int val) {
    return "[" + to_string(lineNo) + "] Error at position " + 
        to_string(pos) + ": " + msg + " {" + to_string(val) + "}\n";
}

int main(int argc, char *argv[]) {
    deque<std::string> inFiles;
    bool showPrompt = true;
    bool prettyPrint = false;
    for (int i = 1; i < argc; i++)
        switch ( (argv[i])[0]) {
            case '-': if ((argv[i])[1] == 'p') prettyPrint = true;
                      else { std::cerr << "invalid option " << (argv[i])[1] << std::endl; exit(1); }
                      break;
            default: inFiles.push_back(std::string(argv[i]));
        }
    // default to std::cin if no files passed
    if (inFiles.empty()) inFiles.push_back("");

    while (!inFiles.empty()) {
        std::ifstream inFile;
        std::istream* inFileP = &std::cin;

        std::string file = inFiles.front();
        // open passed file or cin if no good file
        if (file != "") {
            inFile.open(file);
            if (inFile.good()) { 
                inFileP = &inFile; 
                showPrompt = false; 
                std::cerr << "Solving " << file << " ..." << std::endl;
            }
            else {
                inFile.close();
                continue;
            }
        } 

        inFiles.pop_front();

        string line = "";
        int lineNo = 0;
        if (showPrompt) output << fmtPrompt(lineNo);
        if(prettyPrint) output << pretty;
        while (getline(*inFileP, line)) {
            ++lineNo;
            try {
                Solver b(line);
               
                if (prettyPrint) output << fmtTitle(lineNo, "Solution:"); 
                if (b.isSolved()) {
                    output << b.solution();
                    std::cerr << fmtTitle(lineNo,"solved without backtracing");
                    continue;
                }
                if (b.backtrackSearch()) output << b.solution();
                else std::cerr << fmtTitle(lineNo, "UNSOLVED !!!!!!!!!!!!!!!!!!!!!!");
            } catch(const InputEx & e) {
                output << fmtError(lineNo, e.getIndex(), e.getMsg(), e.getValue());
            }
            if (showPrompt) output << fmtPrompt(lineNo);
        }
        inFile.close();
    }
}
