//
// Created by GUYYY on 14/04/2025.
//

#include "Rook.h"

Rook::Rook(char symbol) : Piece(symbol) { }

bool Rook::isMoveLegal(int srcRow, int srcCol, int destRow, int destCol,
                        Piece *board[8][8]) const
{
    if (srcRow != destRow && srcCol != destCol)
        return false;

    int rowDir = (destRow > srcRow) ? 1 : (destRow < srcRow) ? -1 : 0;
    int colDir = (destCol > srcCol) ? 1 : (destCol < srcCol) ? -1 : 0;

    int row = srcRow + rowDir;
    int col = srcCol + colDir;

    while (row != destRow || col != destCol) {
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
