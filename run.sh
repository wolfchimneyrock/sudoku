#!/bin/bash
output=${1-sudokus}
if ! [ -s ./sudoku ]; then
    make sudoku
else
    time ./sudoku $output.txt > ${output}_solved.txt
fi
