#ifndef BISHOP_H
#define BISHOP_H

#include "Piece.h"

class Bishop : public Piece {
public:
    Bishop(bool isWhite) : Piece(isWhite) {}

    char getSymbol() const override {
        return isWhite ? 'B' : 'b';
    }

    bool isValidMove(int srcRow, int srcCol, int destRow, int destCol, const Board& board) const override;
};

#endif // BISHOP_H
