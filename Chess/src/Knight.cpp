#include "Knight.h"
#include <cmath>

bool Knight::isValidMove(int srcRow, int srcCol, int destRow, int destCol, const Board& board) const {
    int rowDiff = std::abs(destRow - srcRow);
    int colDiff = std::abs(destCol - srcCol);

    // L-shape movement
    return (rowDiff == 2 && colDiff == 1) || (rowDiff == 1 && colDiff == 2);
}
