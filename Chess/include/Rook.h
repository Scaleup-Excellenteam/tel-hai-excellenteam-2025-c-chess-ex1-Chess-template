#ifndef ROOK_H
#define ROOK_H

#include "Piece.h"

class Rook : public Piece {
private:
    bool isWhite;

public:
    Rook(bool isWhite);
    virtual bool isValidMove(int fromX, int fromY, int toX, int toY, const std::string& board) const override;
    virtual char getSymbol() const override;
};

#endif
