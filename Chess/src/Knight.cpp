#include "Knight.h"
#include <cmath>
#include <cctype>

bool Knight::is_legel_movement(const string &input, const string &board, bool isWhiteTurn) {
    int col_source = input[0] - 'a';
    int row_source = 8 - (input[1] - '0');
    int col_dest = input[2] - 'a';
    int row_dest = 8 - (input[3] - '0');
    int dx = abs(col_dest - col_source);
    int dy = abs(row_dest - row_source);
    if (!((dx == 2 && dy == 1) || (dx == 1 && dy == 2)))
        return false;

    char destPiece = board[row_dest * 8 + col_dest];
    // Check destination: allow empty or opponent’s piece
    if (destPiece == '#')
        return true;
    if (isWhiteTurn && isupper(destPiece))
        return false;
    if (!isWhiteTurn && islower(destPiece))
        return false;
    return true;
}

char Knight::get_type() {
    return type;
}
