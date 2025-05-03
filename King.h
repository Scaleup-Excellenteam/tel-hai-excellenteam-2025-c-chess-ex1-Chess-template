//
// Created by MyPC on 12/04/2025.
//

#ifndef HOMEWORK01EX_KING_H
#define HOMEWORK01EX_KING_H
#pragma once
#include "Piece.h"

class King : public Piece {
    bool white;
public:
    King(bool isWhite);
    bool isWhite() const override;
    char symbol() const override;
    bool isLegalMove(int srcRow, int srcCol, int destRow, int destCol,
                     const std::vector<std::vector<Piece*>>& board) const override;
};
#endif //HOMEWORK01EX_KING_H
