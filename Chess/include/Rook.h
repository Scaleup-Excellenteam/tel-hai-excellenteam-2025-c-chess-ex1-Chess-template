#ifndef ROOK_H
#define ROOK_H

#include "Piece.h"
#include <vector>
#include <utility>


class Rook : public Piece {
    char symbol;
public:
    Rook(char symbol); 
    Rook(char s, bool isWhite);
    char getSymbol() const override;
    bool isValidMove(int fromX, int fromY, int toX, int toY, const Piece* const board[8][8]) const override;
    std::vector<std::pair<int, int>> getLegalMoves(int x, int y, const Piece* const board[8][8]) const override;
    Piece* clone() const override ;
        


};

#endif
