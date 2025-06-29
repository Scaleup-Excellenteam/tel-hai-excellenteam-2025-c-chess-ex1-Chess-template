
#include "Queen.h"
#include "Rook.h"
#include "Bishop.h"
#include <cmath>

Queen::Queen(bool isWhite) : white(isWhite) {}

bool Queen::isWhite() const { return white; }

char Queen::symbol() const { return white ? 'Q' : 'q'; }

bool Queen::isLegalMove(int srcRow, int srcCol, int destRow, int destCol,
                        const std::vector<std::vector<Piece*>>& board) const {
    if (srcRow == destRow || srcCol == destCol) {
        Rook temp(white);
        return temp.isLegalMove(srcRow, srcCol, destRow, destCol, board);
    } else if (std::abs(destRow - srcRow) == std::abs(destCol - srcCol)) {
        Bishop temp(white);
        return temp.isLegalMove(srcRow, srcCol, destRow, destCol, board);
    }
    return false;
}
