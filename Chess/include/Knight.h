//
// Created by mohamamdlahwani on 4/12/25.
//

#ifndef KNIGHT_H
#define KNIGHT_H
#pragma once
#include "Piece.h"

class knight : public Piece {
public:
    knight(bool isWhite) : Piece(isWhite) {}

    bool isLegalMove(int srcRow, int srcCol, int dstRow, int dstCol, Piece* board[8][8]) override;
    char getSymbol() const override {
        return isWhite ? 'N' : 'n';
    }
};

#endif //KNIGHT_H
