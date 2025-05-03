
#include "Bishop.h"
#include <cmath>

Bishop::Bishop(bool isWhite) : white(isWhite) {}

bool Bishop::isWhite() const { return white; }

char Bishop::symbol() const { return white ? 'B' : 'b'; }

bool Bishop::isLegalMove(int srcRow, int srcCol, int destRow, int destCol,
                         const std::vector<std::vector<Piece*>>& board) const {
    if (std::abs(destRow - srcRow) != std::abs(destCol - srcCol))
        return false;

    int stepRow = (destRow > srcRow) ? 1 : -1;
    int stepCol = (destCol > srcCol) ? 1 : -1;
    int row = srcRow + stepRow;
    int col = srcCol + stepCol;

    while (row != destRow && col != destCol) {
        if (board[row][col] != nullptr)
            return false;
        row += stepRow;
        col += stepCol;
    }

    if (board[destRow][destCol] != nullptr && board[destRow][destCol]->isWhite() == white)
        return false;

    return true;
}