#include "Knight.h"
#include <cmath>

bool Knight::isValidMove(int srcRow, int srcCol, int destRow, int destCol, const Board&) const {
    int dx = abs(destRow - srcRow);
    int dy = abs(destCol - srcCol);
    return (dx == 2 && dy == 1) || (dx == 1 && dy == 2);
}
