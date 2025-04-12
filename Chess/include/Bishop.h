//
// Created by mohamamdlahwani on 4/7/25.
//

#ifndef BISHOP_H
#define BISHOP_H
// Bishop.h
#pragma once
#include "Piece.h"

class Bishop : public Piece {
public:
    Bishop(bool isWhite) : Piece(isWhite) {}

    bool isLegalMove(int srcRow, int srcCol, int dstRow, int dstCol, Piece* board[8][8]) override;
    char getSymbol() const override {
        return isWhite ? 'B' : 'b';
    }
};
#endif //BISHOP_H
