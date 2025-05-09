//
// Created by GUYYY on 14/04/2025.
//

#include "Bishop.h"
#include <cmath>

Bishop::Bishop(char symbol) : Piece(symbol) { }

bool Bishop::isMoveLegal(int srcRow, int srcCol, int destRow, int destCol,
                         Piece* board[8][8]) const {
    int d = std::abs(destRow - srcRow);
    if (d != std::abs(destCol - srcCol) || d == 0) // added d == 0
        return false;

    int rowDir = (destRow - srcRow) / d;
    int colDir = (destCol - srcCol) / d;

    for (int i = 1; i < d; ++i) {
        if (board[srcRow + i * rowDir][srcCol + i * colDir] != nullptr)
            return false;
    }

    const Piece* destPiece = board[destRow][destCol];
    if (destPiece != nullptr && destPiece->getColor() == this->getColor())
        return false;

    return true;
}