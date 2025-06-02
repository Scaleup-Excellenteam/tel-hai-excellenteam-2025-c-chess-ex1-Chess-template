#include "Knight.h"
#include <cmath>

bool Knight::is_legel_movement(const std::string& input,
                               const std::string& board,
                               bool isWhiteTurn)
{
    int srcCol = input[0] - 'a';
    int srcRow = '8' - input[1];
    int dstCol = input[2] - 'a';
    int dstRow = '8' - input[3];

    int dr = std::abs(dstRow - srcRow);
    int dc = std::abs(dstCol - srcCol);

    // Knights move exactly 2×1 or 1×2
    return ( (dr == 2 && dc == 1) || (dr == 1 && dc == 2) );
}

char Knight::get_type() {
    return type;
}
