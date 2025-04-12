//
// Created by mohamamdlahwani on 4/7/25.
//

#ifndef ROOK_H
#define ROOK_H



// Rook.h
#pragma once
#include "Piece.h"

class Rook : public Piece {
public:
    Rook(bool isWhite) : Piece(isWhite) {}

    bool isLegalMove(int srcRow, int srcCol, int dstRow, int dstCol, Piece* board[8][8]) override;
    char getSymbol() const override {
        return isWhite ? 'R' : 'r';
    }
};



#endif //ROOK_H
