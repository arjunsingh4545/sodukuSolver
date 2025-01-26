//
// Created by 91704 on 26-01-2025.
//

#include "soduku.h"

bool soduku::set(int i, int j, int value) {
    cell& c = cells[i][j];
    if (c.value == value) return true;
    if (c.constraints.test(value)) return false;
    //update state of cell
    c.constraints = std::bitset<10>(0x3FE); // reset constraints
    c.constraints.reset(value); // set value as allowed
    c.numPossibilities = 1;
    c.value = value;

    //propagate constraints

    for(int k=0; k < 9 ; k++) {
        if ( i != k && !updateConstraints(k,j,value)) return false;
        if ( j != k && !updateConstraints(i,k,value)) return false;
        // 3x3 subgrid
        int ix = (i/3)*3 + k/3;
        int jx = (j/3)*3 + k%3;
        if (ix != i && jx != j && !updateConstraints(ix,jx,value)) return false;
    }
    return true;
}

bool soduku::updateConstraints(int i, int j, int excludeValue) {
    cell& c = cells[i][j];
    if (c.constraints.test(excludeValue)) return true;
    if (c.value == excludeValue) return false;
    c.constraints.set(excludeValue);
    if (--c.numPossibilities > 1) return true;
    for(int v = 1; v <= 9; v++) {
        if (!c.constraints.test(v)) {
            return set(i,j,v);
        }
    }
    assert(false); // debugging purposes
}

bool soduku::findValuesForEmptyCells() {
    bt.clear();
    for(int i=0 ; i < 9 ; i++) {
        for(int j=0 ; j < 9 ; j++) {
            if (cells[i][j].value == 0) {
                bt.emplace_back(std::make_pair(i,j));
            }
        }
    }
    // making backtracking efficient by pre-sorting empty cells by numPossibilities
    std::sort(bt.begin(), bt.end(), [this](const std::pair<int, int>&a, const std::pair<int, int>&b) {
        return cells[a.first][a.second].numPossibilities < cells[b.first][b.second].numPossibilities; });
    return backtrack(0);
}

bool soduku::backtrack(int pos) {
    if(pos >= bt.size()) return true;

    int i=bt[pos].first;
    int j=bt[pos].second;
    // if the cell is already set, proceed to next cell
    if(cells[i][j].value) return backtrack(pos+1);

    auto constraint = cells[i][j].constraints;
    std::array<std::array<cell, 9>, 9> snapshot(cells);

    for(int v =1 ; v <= 9 ; v++) {
        if (!constraint.test(v)) {
            if (set(i,j,v)) {
                if (backtrack(pos+1)) return true;
            }
            cells = snapshot;
        }
    }
    return false;
}

void soduku::solveSoduku() {
    cells = std::array<std::array<cell,9>,9>(); // clear array
    // Decoding input board into the internal cell matrix.
    // As we do it - constraints are propagated and even additional values are set as we go
    // (in the case if it is possible to unambiguously deduce them).
    for (int i = 0; i < 9; i++)
    {
        for (int j = 0; j < 9; j++) {
            if (board[i][j] != '.' && !set(i, j, board[i][j] - '0'))
                return; // sudoku is either incorrect or unsolvable
        }
    }
    // if we're lucky we've already got a solution,
    // however, if we have empty cells we need to use backtracking to fill them
    if (!findValuesForEmptyCells())
        return; // sudoku is unsolvable

    // copying the solution back to the board
    for (int i = 0; i < 9; i++)
    {
        for (int j = 0; j < 9; j++) {
            if (cells[i][j].value)
                board[i][j] = cells[i][j].value + '0';
        }
    }
}

soduku::soduku(std::vector<std::vector<char>> &board) {
    this->board = board;
}
