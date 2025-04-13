#pragma once
#include "Piece.h"

struct indexPair;
class Knight : public Piece
{
public:
    Knight(bool isWhite) : Piece(isWhite) {};
    virtual bool checkMoveRange(const indexPair& source, const indexPair& destination, const Board& board) const override;

private:
    static bool registerIt;
};