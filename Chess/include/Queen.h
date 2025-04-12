//
// Created by mohamamdlahwani on 4/7/25.
//

#ifndef QUEEN_H
#define QUEEN_H
// Queen.h
#pragma once
#include "Piece.h"

class Queen : public Piece {
public:
    Queen(bool isWhite) : Piece(isWhite) {}

    bool isLegalMove(int srcRow, int srcCol, int dstRow, int dstCol, Piece* board[8][8]) override;
    char getSymbol() const override {
        return isWhite ? 'Q' : 'q';
    }
};
#endif //QUEEN_H
