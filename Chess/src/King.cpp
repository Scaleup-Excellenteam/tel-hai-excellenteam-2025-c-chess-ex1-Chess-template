//
// Created by GUYYY on 14/04/2025.
//

#include "King.h"

King::King(char symbol) : Piece(symbol) { }

bool King::isMoveLegal(int srcRow, int srcCol, int destRow, int destCol,
                       Piece* board[8][8]) const
{
    int rowDiff = abs(destRow - srcRow);
    int colDiff = abs(destCol - srcCol);

    if (rowDiff <= 1 && colDiff <= 1) {
        const Piece* destPiece = board[destRow][destCol];
        if (destPiece != nullptr && destPiece->getColor() == this->getColor())
            return false;

        return true;
    }

    return false;
}
