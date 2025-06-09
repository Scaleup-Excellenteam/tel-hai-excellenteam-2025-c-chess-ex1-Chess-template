//
// Created by GUYYY on 14/04/2025.
//

#ifndef BISHOP_H
#define BISHOP_H

#include "Piece.h"

class Bishop : public Piece {
public:
    Bishop(char symbol);

    bool isMoveLegal(int srcRow, int srcCol, int destRow, int destCol,
                     Piece* board[8][8]) const override;

    Bishop* clone() const override {
        return new Bishop(_symbol);
    }
};



#endif //BISHOP_H
