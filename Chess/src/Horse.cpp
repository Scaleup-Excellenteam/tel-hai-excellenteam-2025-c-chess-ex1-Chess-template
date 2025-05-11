#include "../include/Horse.h"
#include <cmath>

Horse::Horse(char s) : symbol(s) {}

Horse::Horse(char s, bool isWhite) : Piece(isWhite), symbol(s) {}

char Horse::getSymbol() const {
    return symbol;
}

bool Horse::isValidMove(int fromX, int fromY, int toX, int toY, const Piece* const board[8][8]) const {
   
    int dx = abs(toX - fromX); 
    int dy = abs(toY - fromY); 

    return (dx == 2 && dy == 1) || (dx == 1 && dy == 2);
}