// Pawn.cpp
#include "Pawn.h"
#include <cctype>
#include <cmath>

bool Pawn::is_legel_movement(const std::string& input,const std::string& board,bool isWhiteTurn) {
    int srcCol = input[0] - 'a';
    int srcRow = input[1] - '1';
    int dstCol = input[2] - 'a';
    int dstRow = input[3] - '1';
    int direction = isWhiteTurn ? +1 : -1;
    int startRow  = isWhiteTurn ? 1  : 6;
    auto at = [&](int row, int col) {
        return board[row * 8 + col];
    };
    if (dstCol == srcCol) {
        if (dstRow == srcRow + direction &&
            at(dstRow, dstCol) == '#') {
            return true;
        }
        if (srcRow == startRow &&
            dstRow == srcRow + 2*direction &&
            at(srcRow + direction, srcCol) == '#' &&
            at(dstRow,             dstCol) == '#') {
            return true;
        }
    }
    if (std::abs(dstCol - srcCol) == 1 &&
        dstRow == srcRow + direction) {
        char target = at(dstRow, dstCol);
        if (target != '#') {
            bool targetIsWhite = std::isupper(target);
            if (targetIsWhite != isWhiteTurn) {
                return true;
            }
        }
    }
    return false;
}

char Pawn::get_type() {
    return type;
}
