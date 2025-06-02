#include "King.h"
#include <cmath>

bool King::is_legel_movement(const std::string& input,
                             const std::string& board,
                             bool isWhiteTurn)
{
    int srcCol = input[0] - 'a';
    int srcRow = '8' - input[1];
    int dstCol = input[2] - 'a';
    int dstRow = '8' - input[3];

    int dr = std::abs(dstRow - srcRow);
    int dc = std::abs(dstCol - srcCol);

    // King moves exactly one square in any direction (including diagonals)
    if ((dr == 1 && dc == 0) || (dr == 0 && dc == 1) || (dr == 1 && dc == 1)) {
        return true;
    }

    // (If you later want to handle castling, check here:
    //   if (srcRow==7 && srcCol==4 && dstRow==7 && (dstCol==2||dstCol==6)) { … } )

    return false;
}

char King::get_type() {
    return type;
}
