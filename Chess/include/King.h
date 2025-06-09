//
// Created by GUYYY on 14/04/2025.
//

#ifndef KING_H
#define KING_H

#include "Piece.h"
#include <cstdlib>

class King : public Piece{
public:
    King(char symbol);

    bool isMoveLegal(int srcRow, int srcCol, int destRow, int destCol,
                     Piece* board[8][8]) const override;

    King* clone() const override {
        return new King(_symbol);
    }
};



#endif //KING_H
