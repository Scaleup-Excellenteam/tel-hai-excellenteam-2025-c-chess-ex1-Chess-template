#pragma once
#include "Piece.h"

struct indexPair;
class Pawn : public Piece
{
public:
    Pawn(bool isWhite) : Piece(isWhite) {};
    virtual bool checkMoveRange(const indexPair& source, const indexPair& destination, const Board& board) const override;

private:
    bool m_hasMoved = false;
    static bool registerIt;
};
