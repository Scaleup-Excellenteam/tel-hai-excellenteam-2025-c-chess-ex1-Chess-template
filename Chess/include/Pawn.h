//
// Created by mohamamdlahwani on 4/12/25.
//

// Pawn.h
#ifndef PAWN_H
#define PAWN_H

#include "Piece.h"

class Pawn : public Piece {
public:
    Pawn(bool isWhite) : Piece(isWhite) {}

    bool isLegalMove(int srcRow, int srcCol, int dstRow, int dstCol, Piece* board[8][8]) override {
        int startRow = isWhite ? 1 : 6;
        int direction = isWhite ? 1 : -1;


        // Move forward
        if (srcCol == dstCol) {
            // One step forward
            if (dstRow - srcRow == direction && board[dstRow][dstCol] == nullptr)
                return true;
            // Two steps from starting position
            if (srcRow == startRow && dstRow - srcRow == 2 * direction &&
                board[srcRow + direction][dstCol] == nullptr && board[dstRow][dstCol] == nullptr)
                return true;
        }
        // Diagonal capture
        if (abs(srcCol - dstCol) == 1 && dstRow - srcRow == direction && board[dstRow][dstCol] != nullptr) {
            if (board[dstRow][dstCol]->isWhite != this->isWhite)
                return true;
        }

        return false;
    }

    char getSymbol() const override {
        return isWhite ? 'P' : 'p';
    }
};

#endif // PAWN_H
