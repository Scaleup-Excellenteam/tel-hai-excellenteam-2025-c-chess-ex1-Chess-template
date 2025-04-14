//
// Created by GUYYY on 14/04/2025.
//

#include "Bishop.h"
#include <cmath>

Bishop::Bishop(char symbol) : Piece(symbol) { }

bool Bishop::isMoveLegal(int srcRow, int srcCol, int destRow, int destCol,
                         Piece* board[8][8]) const
{
    int dRow = std::abs(destRow - srcRow);
    int dCol = std::abs(destCol - srcCol);

    if (dRow != dCol)
        return false;

    int rowDir = (destRow > srcRow) ? 1 : -1;
    int colDir = (destCol > srcCol) ? 1 : -1;

    int row = srcRow + rowDir;
    int col = srcCol + colDir;

    while (row != destRow && col != destCol) {
        if (board[row][col] != nullptr)
            return false;

        row += rowDir;
        col += colDir;
    }

    const Piece* destPiece = board[destRow][destCol];
    if (destPiece != nullptr && destPiece->getColor() == this->getColor())
        return false;

    return true;
}