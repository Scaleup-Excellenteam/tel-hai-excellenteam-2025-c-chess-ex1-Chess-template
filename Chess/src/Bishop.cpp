#include "Bishop.h"
#include <cmath>

bool Bishop::is_legel_movement(const std::string& input,
                               const std::string& board,
                               bool isWhiteTurn)
{
    int srcCol = input[0] - 'a';
    int srcRow = '8' - input[1];
    int dstCol = input[2] - 'a';
    int dstRow = '8' - input[3];

    int dr = std::abs(dstRow - srcRow);
    int dc = std::abs(dstCol - srcCol);

    // Must move diagonally any positive distance (dr == dc && dr>0)
    return (dr == dc && dr > 0);
}

char Bishop::get_type() {
    return type;
}
