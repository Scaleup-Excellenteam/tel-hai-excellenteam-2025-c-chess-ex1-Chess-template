#include "King.h"
#include "Board.h"
#include <cmath>

// King moves one square in any direction
bool King::isValidMove(int srcRow, int srcCol, int destRow, int destCol, const Board& board) const {
    int rowDiff = std::abs(destRow - srcRow);
    int colDiff = std::abs(destCol - srcCol);

    if ((rowDiff <= 1) && (colDiff <= 1)) {
        return true;
    }

    return false;
}
