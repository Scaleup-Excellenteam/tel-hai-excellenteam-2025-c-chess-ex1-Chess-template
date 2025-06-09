//
// Created by GUYYY on 14/04/2025.
//

#include "King.h"
#include "Rook.h"
#include <cmath>

King::King(char symbol) : Piece(symbol) { }

bool King::isMoveLegal(int srcRow, int srcCol, int destRow, int destCol,
                       Piece* board[8][8]) const
{
    int rowDiff = std::abs(destRow - srcRow);
    int colDiff = std::abs(destCol - srcCol);

    if (rowDiff <= 1 && colDiff <= 1) {
        const Piece* destPiece = board[destRow][destCol];
        if (destPiece && destPiece->getColor() == this->getColor())
            return false;
        return true;
    }

    // --- check for castling ---
    if (rowDiff == 0 && colDiff == 2 && !this->getHasMoved()) {
        int direction = (destCol - srcCol > 0) ? 1 : -1;
        int rookCol = (direction == 1) ? 7 : 0;
        Piece* rook = board[srcRow][rookCol];

        if (!rook || (rook->getSymbol() != 'R' && rook->getSymbol() != 'r') ||
            rook->getColor() != this->getColor() || rook->getHasMoved())
            return false;

        for (int c = srcCol + direction; c != rookCol; c += direction)
            if (board[srcRow][c] != nullptr)
                return false;


        return true;
    }

    return false;
}