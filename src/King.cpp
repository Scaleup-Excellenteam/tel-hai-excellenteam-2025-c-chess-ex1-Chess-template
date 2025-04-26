#include "King.h"
#include <cmath>

bool King::isValidMove(int srcRow, int srcCol, int destRow, int destCol, const Board&) const {
    return std::abs(destRow - srcRow) <= 1 && std::abs(destCol - srcCol) <= 1;
}
