#include <iostream>
#include "soduku.h"
int main(int argc , char* argv[]) {
    std::vector<std::vector<char>> board;// input board
    soduku s(board);
    s.solveSoduku();
    return 0;
}
