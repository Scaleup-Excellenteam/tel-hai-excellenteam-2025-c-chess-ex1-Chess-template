#ifndef QUEEN_H
#define QUEEN_H

#include "Piece.h"

class Queen : public Piece {
public:
    Queen(bool isWhite) : Piece(isWhite) {}

    char getSymbol() const override {
        return isWhite ? 'Q' : 'q';
    }

    bool isValidMove(int srcRow, int srcCol, int destRow, int destCol, const Board& board) const override;
};

#endif // QUEEN_H
