#include "Bishop.h"


bool Bishop::is_legel_movement(const std::string &input, const std::string &board, bool isWhiteTurn) {
    int col_source = input[0] - 'a';
    int row_source = input[1] - '1';
    int col_dest   = input[2] - 'a';
    int row_dest   = input[3] - '1';
    int row_diff = row_dest - row_source;
    int col_diff = col_dest - col_source;
    if (std::abs(row_diff) != std::abs(col_diff))
        return false;
    int row_step = (row_diff > 0) ? 1 : -1;
    int col_step = (col_diff > 0) ? 1 : -1;
    int current_row = row_source + row_step;
    int current_col = col_source + col_step;
    while (current_row != row_dest && current_col != col_dest) {
        if (board[(7 - current_row) * 8 + current_col] != '#') {
            return false;
        }
        current_row += row_step;
        current_col += col_step;
    }
    char destPiece = board[(7 - row_dest) * 8 + col_dest];
    if (destPiece == '#')
        return true;
    if (isWhiteTurn && std::isupper(destPiece))
        return false;
    if (!isWhiteTurn && std::islower(destPiece))
        return false;
    return true;
}

char Bishop::get_type() {
    return type;
}
