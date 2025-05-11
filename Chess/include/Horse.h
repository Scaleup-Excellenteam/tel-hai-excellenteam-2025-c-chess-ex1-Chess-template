#ifndef HORSE_H
#define HORSE_H

#include "Piece.h"

class Horse : public Piece {
    char symbol;
public:
    Horse(char symbol); 
    Horse(char s, bool isWhite);
    char getSymbol() const override;
    bool isValidMove(int fromX, int fromY, int toX, int toY, const Piece* const board[8][8]) const override;
   
};

#endif
