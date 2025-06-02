#include "Queen.h"
#include <cmath>

bool Queen::is_legel_movement(const std::string& input,
                              const std::string& board,
                              bool isWhiteTurn)
{
    int srcCol = input[0] - 'a';
    int srcRow = '8' - input[1];
    int dstCol = input[2] - 'a';
    int dstRow = '8' - input[3];

    int dr = std::abs(dstRow - srcRow);
    int dc = std::abs(dstCol - srcCol);

    // Queen moves like Rook OR Bishop:
    bool straight = (srcCol == dstCol && srcRow != dstRow) ||
                    (srcRow == dstRow && srcCol != dstCol);
    bool diagonal = (dr == dc && dr > 0);

    return straight || diagonal;
}

char Queen::get_type() {
    return type;
}
