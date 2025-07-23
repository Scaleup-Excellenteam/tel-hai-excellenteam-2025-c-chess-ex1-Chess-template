#include "King.h"
#include <cmath>
#include <cctype>

bool King::is_legel_movement(const string &input, const string &board, bool isWhiteTurn) {
    int col_source = input[0] - 'a';
    int row_source = 8 - (input[1] - '0');
    int col_dest = input[2] - 'a';
    int row_dest = 8 - (input[3] - '0');
    int rowDiff = abs(row_source - row_dest);
    int colDiff = abs(col_source - col_dest);
    // King moves only 1 square in any direction
    if (rowDiff > 1 || colDiff > 1)
        return false;
    char destPiece = board[row_dest * 8 + col_dest];
    if (destPiece == '#')
        return true;
    if (isWhiteTurn && isupper(destPiece))
        return false;
    if (!isWhiteTurn && islower(destPiece))
        return false;
    return true;
}

char King::get_type() {
    return type;
}