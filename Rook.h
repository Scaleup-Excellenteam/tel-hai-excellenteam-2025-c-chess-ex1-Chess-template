
#ifndef HOMEWORK01EX_ROOK_H
#define HOMEWORK01EX_ROOK_H
#pragma once
#include "Piece.h"

class Rook : public Piece {
    bool white;
public:
    Rook(bool isWhite);
    bool isWhite() const override;
    char symbol() const override;
    bool isLegalMove(int srcRow, int srcCol, int destRow, int destCol,
                     const std::vector<std::vector<Piece*>>& board) const override;
};

#endif //HOMEWORK01EX_ROOK_H
