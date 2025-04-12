// Rook.cpp
#include "Rook.h"

bool Rook::isLegalMove(int srcRow, int srcCol, int dstRow, int dstCol, Piece* board[8][8]) {
    if (srcRow != dstRow && srcCol != dstCol)
        return false; // must move in straight line

    int rowStep = (dstRow > srcRow) - (dstRow < srcRow);
    int colStep = (dstCol > srcCol) - (dstCol < srcCol);

    int row = srcRow + rowStep, col = srcCol + colStep;
    while (row != dstRow || col != dstCol) {
        if (board[row][col] != nullptr)
            return false; // path blocked
        row += rowStep;
        col += colStep;
    }

    return (board[dstRow][dstCol] == nullptr || board[dstRow][dstCol]->isWhite != isWhite);
}
