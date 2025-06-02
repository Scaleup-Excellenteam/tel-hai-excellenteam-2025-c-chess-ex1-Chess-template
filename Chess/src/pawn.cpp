#include "Pawn.h"
#include <cctype>
#include <cmath>

bool Pawn::is_legel_movement(const std::string& input,
                             const std::string& board,
                             bool isWhiteTurn)
{
    // 1) Convert algebraic (“e2e4”) to 0-based indices:
    int srcCol = input[0] - 'a';
    int srcRow = '8' - input[1];
    int dstCol = input[2] - 'a';
    int dstRow = '8' - input[3];

    // 2) Pawn moves “forward” one square or two from starting rank
    int direction = isWhiteTurn ? -1 : +1;
    int startRow  = isWhiteTurn ? 6  : 1;

    // 3) Helper to look at the 64-char string (row*8 + col)
    auto at = [&](int r, int c) -> char {
        return board[r * 8 + c];
    };

    // 4) Single‐step forward if empty
    if (dstCol == srcCol &&
        dstRow == srcRow + direction &&
        at(dstRow, dstCol) == '#')
    {
        return true;
    }

    // 5) Double‐step from start if both squares empty
    if (srcRow == startRow &&
        dstCol == srcCol &&
        dstRow == srcRow + 2 * direction &&
        at(srcRow + direction, srcCol) == '#' &&
        at(dstRow,            dstCol) == '#')
    {
        return true;
    }

    // 6) Diagonal capture one step forward
    if (std::abs(dstCol - srcCol) == 1 &&
        dstRow == srcRow + direction)
    {
        char target = at(dstRow, dstCol);
        if (target != '#') {
            bool targetIsWhite = std::isupper(target);
            if (targetIsWhite != isWhiteTurn) {
                return true;
            }
        }
    }

    // 7) Otherwise illegal
    return false;
}

char Pawn::get_type() {
    return type;
}
