

#ifndef HOMEWORK01EX_QUEEN_H
#define HOMEWORK01EX_QUEEN_H
#pragma once
#include "Piece.h"

class Queen : public Piece {
    bool white;
public:
    Queen(bool isWhite);
    bool isWhite() const override;
    char symbol() const override;
    bool isLegalMove(int srcRow, int srcCol, int destRow, int destCol,
                     const std::vector<std::vector<Piece*>>& board) const override;
};

#endif //HOMEWORK01EX_QUEEN_H
