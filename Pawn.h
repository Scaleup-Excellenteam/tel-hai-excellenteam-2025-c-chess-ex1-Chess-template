

#ifndef HOMEWORK01EX_PAWN_H
#define HOMEWORK01EX_PAWN_H
#pragma once
#include "Piece.h"

class Pawn : public Piece {
    bool white;
public:
    Pawn(bool isWhite);
    bool isWhite() const override;
    char symbol() const override;
    bool isLegalMove(int srcRow, int srcCol, int destRow, int destCol,
                     const std::vector<std::vector<Piece*>>& board) const override;
};

#endif //HOMEWORK01EX_PAWN_H
