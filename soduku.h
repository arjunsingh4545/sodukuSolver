//
// Created by 91704 on 26-01-2025.
//

#ifndef SODUKUSOLVER_SODUKU_H
#define SODUKUSOLVER_SODUKU_H


#include <cstdint>
#include <bitset>
#include <array>
#include <vector>
#include <cassert>
#include <algorithm>
class soduku {
private:
    struct cell {
        // cell on the board
        // value is the number in the cell, 0 if not set
        uint8_t value;
        uint8_t numPossibilities; // number of possibilities for the cell
        std::bitset<10> constraints; // bitset of the numbers that can't be in the cell
        cell() : value(0), numPossibilities(9), constraints() {}
    };
    std::vector<std::vector<char>> board;
    //using array instead of vector as size of board is fixed , will not be altered during runtime , and array is faster than vector
    std::array<std::array<cell, 9>, 9> cells;
    std::vector<std::pair<int, int>> bt; // backtracking stack , stores positions  of empty cells for bt;

    // helper functions
    bool set(int i  , int  j, int value); // i = row , j = column , v = value
    // if cell already has the value , it returns true
    // if value is not allowed due to constraints , it returns false
    // if value is set successfully , it updates the constraints for other cells ,  returns true

    bool updateConstraints(int i , int j , int excludeValue); // i = row , j = column , v = value
    // updates constrains by elimintating excludeValue from the constraints of the cell at i,j
    //If the cell has only one possibility left after updating constraints, it recursively calls set() to fill that cell.

    bool findValuesForEmptyCells(); // This function collects all empty cells and sorts them by the number of possibilities to optimize the backtracking process.
    //  It calls the backtrack() function to fill in the empty cells.

    bool backtrack(int pos); // pos = position in the backtracking stack
    //This function attempts to fill the empty cells starting from index k.
    //If a cell has only one possibility, it proceeds to the next cell.
    //If there are multiple possibilities, it tries each possibility and uses a snapshot of the current state to restore if a guess fails.
public:
    explicit soduku(std::vector<std::vector<char>>& board);
    void solveSoduku(); // board = 9x9 soduku board
    // assuming the input board is a 2D vector of characters, where '.' represents an empty cell.
};


#endif //SODUKUSOLVER_SODUKU_H
