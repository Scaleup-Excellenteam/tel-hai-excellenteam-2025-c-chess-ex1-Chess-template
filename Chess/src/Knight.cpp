//
// Created by GUYYY on 14/04/2025.
//

#include "Knight.h"
#include <cmath>

Knight::Knight(char symbol)
    : Piece(symbol)
{}

bool Knight::isMoveLegal(int srcRow, int srcCol, int destRow, int destCol,
                         Piece* board[8][8]) const
{
    int dRow = std::abs(destRow - srcRow);
    int dCol = std::abs(destCol - srcCol);

    if (!((dRow == 2 && dCol == 1) || (dRow == 1 && dCol == 2)))
        return false;

    const Piece* destPiece = board[destRow][destCol];
    if (destPiece != nullptr && destPiece->getColor() == this->getColor())
        return false;

    return true;
}