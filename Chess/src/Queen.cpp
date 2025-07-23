#include "Queen.h"

bool Queen::is_legel_movement(const string &input, const string &board, bool isWhiteTurn) {
    int col_source =input[0]-'a';
    int row_source=8-(input[1]-'0');
    int col_dest=input[2]-'a';
    int row_dest=8-(input[3]-'0');
    int row_diff = row_dest - row_source;
    int col_diff = col_dest - col_source;

    int row_step = (row_diff == 0) ? 0 : (row_diff > 0 ? 1 : -1);
    int col_step = (col_diff == 0) ? 0 : (col_diff > 0 ? 1 : -1);

    // Check if move is legal direction (straight or diagonal)
    if (!(row_source == row_dest || col_source == col_dest || std::abs(row_diff) == std::abs(col_diff)))
        return false;

    // Check the path is clear (excluding the destination)
    int current_row = row_source + row_step;
    int current_col = col_source + col_step;
    while (current_row != row_dest || current_col != col_dest) {
        if (board[current_row * 8 + current_col] != '#')
            return false;
        current_row += row_step;
        current_col += col_step;
    }
    // Check destination
    char destPiece = board[row_dest * 8 + col_dest];
    if (destPiece == '#')
        return true;
    if (isWhiteTurn && std::isupper(destPiece))
        return false;
    if (!isWhiteTurn && std::islower(destPiece))
        return false;
    return true;
}

char Queen::get_type() {
    return type;
}
