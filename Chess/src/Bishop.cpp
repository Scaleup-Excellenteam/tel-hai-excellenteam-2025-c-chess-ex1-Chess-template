//
// Created by mohamamdlahwani on 4/7/25.
//
// Bishop.cpp
#include "Bishop.h"
#include <cmath>

bool Bishop::isLegalMove(int srcRow, int srcCol, int dstRow, int dstCol, Piece* board[8][8]) {
    if (std::abs(dstRow - srcRow) != std::abs(dstCol - srcCol))
        return false; // must move diagonally

    int rowStep = (dstRow > srcRow) - (dstRow < srcRow);
    int colStep = (dstCol > srcCol) - (dstCol < srcCol);

    int row = srcRow + rowStep, col = srcCol + colStep;
    while (row != dstRow && col != dstCol) {
        if (board[row][col] != nullptr)
            return false;
        row += rowStep;
        col += colStep;
    }

    return (board[dstRow][dstCol] == nullptr || board[dstRow][dstCol]->isWhite != isWhite);
}
