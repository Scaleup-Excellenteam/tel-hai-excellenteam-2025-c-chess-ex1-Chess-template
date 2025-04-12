//
// Created by mohamamdlahwani on 4/7/25.
//

#ifndef PIECE_H
#define PIECE_H
// Piece.h
#pragma once


class Piece {
public:
    bool isWhite;

    Piece(bool isWhite) : isWhite(isWhite) {}
    virtual ~Piece() = default;

    virtual bool isLegalMove(int srcRow, int srcCol, int dstRow, int dstCol, Piece* board[8][8]) = 0;
    virtual char getSymbol() const = 0;
};
#endif //PIECE_H
