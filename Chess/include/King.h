//
// Created by mohamamdlahwani on 4/7/25.
//

#ifndef KING_H
#define KING_H
// King.h
#pragma once
#include "Piece.h"

class King : public Piece {
public:
    King(bool isWhite) : Piece(isWhite) {}

    bool isLegalMove(int srcRow, int srcCol, int dstRow, int dstCol, Piece* board[8][8]) override;
    char getSymbol() const override {
        return isWhite ? 'K' : 'k';
    }
};
#endif //KING_H
