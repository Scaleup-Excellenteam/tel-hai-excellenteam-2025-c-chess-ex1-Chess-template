#include "Rook.h"

bool Rook::is_legel_movement(const std::string& input,
                             const std::string& board,
                             bool isWhiteTurn)
{
    int srcCol = input[0] - 'a';
    int srcRow = '8' - input[1];
    int dstCol = input[2] - 'a';
    int dstRow = '8' - input[3];

    // Rook moves straight (either same row or same column), and distance > 0
    return ( (srcCol == dstCol && srcRow != dstRow) ||
             (srcRow == dstRow && srcCol != dstCol) );
}

char Rook::get_type() {
    return type;
}
