//
// Created by GUYYY on 14/04/2025.
//

#include "Pawn.h"
#include <cmath>

Pawn::Pawn(char symbol)
    : Piece(symbol)
{}

bool Pawn::isMoveLegal(int srcRow, int srcCol, int destRow, int destCol,
                       Piece* board[8][8]) const
{
    int dir = (_color == WHITE) ? 1 : -1;
    int startRow = (_color == WHITE) ? 1 : 6;

    int dRow = destRow - srcRow;
    int dCol = std::abs(destCol - srcCol);
    const Piece* destPiece = board[destRow][destCol];

    if (dRow == dir && dCol == 1 && destPiece != nullptr &&
        destPiece->getColor() != this->getColor())
    {
        return true;
    }

    if (dCol == 0)
    {
        if (dRow == dir && destPiece == nullptr)
            return true;

        if (srcRow == startRow && dRow == 2 * dir &&
            board[srcRow + dir][srcCol] == nullptr &&
            destPiece == nullptr)
        {
            return true;
        }
    }

    return false;
}