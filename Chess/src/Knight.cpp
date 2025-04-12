//
// Created by mohamamdlahwani on 4/12/25.
//
#include "Knight.h"
#include <cmath>

bool knight::isLegalMove(int srcRow, int srcCol, int dstRow, int dstCol, Piece* board[8][8]) {
    int rowDiff = std::abs(dstRow - srcRow);
    int colDiff = std::abs(dstCol - srcCol);

    // L-shape movement
    return (rowDiff == 2 && colDiff == 1) || (rowDiff == 1 && colDiff == 2);
}
