#ifndef KING_H
#define KING_H

#include "Piece.h"

class King : public Piece {
public:
    King(bool isWhite) : Piece(isWhite) {}

    char getSymbol() const override {
        return isWhite ? 'K' : 'k';
    }

    bool isValidMove(int srcRow, int srcCol, int destRow, int destCol, const Board& board) const override;
};

#endif // KING_H
