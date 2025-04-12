#include "Rook.h"


bool Rook::is_legel_movement(const string &input, const string &board, bool isWhiteTurn) {
    int col_source =input[0]-'a';
    int row_source=8-(input[1]-'0');
    int col_dest=input[2]-'a';
    int row_dest=8-(input[3]-'0');

    // check if the rook move straight line
    if (row_source != row_dest && col_source != col_dest)
        return false;

    // Check vertical movement
    if (col_source == col_dest) {
        int start = std::min(row_source, row_dest) + 1;
        int end = std::max(row_source, row_dest);
        for (int i = start; i < end; ++i) {
            if (board[i * 8 + col_source] != '#')
                return false;
        }
    }
        // Check horizontal movement
    else if (row_source == row_dest) {
        int start = std::min(col_source, col_dest) + 1;
        int end = std::max(col_source, col_dest);
        for (int i = start; i < end; ++i) {
            if (board[row_source * 8 + i] != '#')
                return false;
        }
    }
    // Check destination piece
    char destPiece = board[row_dest * 8 + col_dest];
    if (destPiece != '#') {
        // If it's same color, not allowed
        if (isWhiteTurn && std::isupper(destPiece))
            return false;
        if (!isWhiteTurn && std::islower(destPiece))
            return false;
    }
    return true;
}


char Rook::get_type() {
   return type;
}
