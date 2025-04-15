#ifndef PAWN_H
#define PAWN_H

#include "Piece.h"

class Pawn : public Piece {
    char symbol;
public:
    Pawn(char symbol); 
    Pawn(char s, bool isWhite);
    char getSymbol() const override;
    bool isValidMove(int fromX, int fromY, int toX, int toY, const Piece* const board[8][8]) const override;
   
};

#endif
