//
// Created by mohamamdlahwani on 4/7/25.
//
// King.cpp
#include "King.h"
#include <cmath>

bool King::isLegalMove(int srcRow, int srcCol, int dstRow, int dstCol, Piece* board[8][8]) {
    int rowDiff = std::abs(dstRow - srcRow);
    int colDiff = std::abs(dstCol - srcCol);

    if (rowDiff <= 1 && colDiff <= 1) {
        if (board[dstRow][dstCol] == nullptr)
            return true;
        return board[dstRow][dstCol]->isWhite != isWhite;
    }
    return false;
}
