
#include "Chess.h"

bool Chess::isInCheck(bool white) {
    std::pair<int, int> kingPos = board.findKing(white);
    for (int r = 0; r < 8; ++r) {
        for (int c = 0; c < 8; ++c) {
            Piece* p = board.getPiece(r, c);
            if (p && p->isWhite() != white) {
                if (p->isLegalMove(r, c, kingPos.first, kingPos.second, board.toString())
                    && p->isClearPath(r, c, kingPos.first, kingPos.second, board.toString())) {
                    return true;
                }
            }
        }
    }
    return false;
}

bool Chess::isCheckmate(bool white) {
    if (!isInCheck(white)) return false;
    for (int r1 = 0; r1 < 8; ++r1) {
        for (int c1 = 0; c1 < 8; ++c1) {
            Piece* p = board.getPiece(r1, c1);
            if (p && p->isWhite() == white) {
                for (int r2 = 0; r2 < 8; ++r2) {
                    for (int c2 = 0; c2 < 8; ++c2) {
                        if ((r1 != r2 || c1 != c2) &&
                            p->isLegalMove(r1, c1, r2, c2, board.toString()) &&
                            p->isClearPath(r1, c1, r2, c2, board.toString())) {
                            Board copy = board;
                            copy.movePiece(r1, c1, r2, c2);
                            Chess temp(copy, !white);
                            if (!temp.isInCheck(white)) return false;
                        }
                    }
                }
            }
        }
    }
    return true;
}

bool Chess::isDraw(bool white) {
    if (isInCheck(white)) return false;
    for (int r1 = 0; r1 < 8; ++r1) {
        for (int c1 = 0; c1 < 8; ++c1) {
            Piece* p = board.getPiece(r1, c1);
            if (p && p->isWhite() == white) {
                for (int r2 = 0; r2 < 8; ++r2) {
                    for (int c2 = 0; c2 < 8; ++c2) {
                        if ((r1 != r2 || c1 != c2) &&
                            p->isLegalMove(r1, c1, r2, c2, board.toString()) &&
                            p->isClearPath(r1, c1, r2, c2, board.toString())) {
                            Board copy = board;
                            copy.movePiece(r1, c1, r2, c2);
                            Chess temp(copy, !white);
                            if (!temp.isInCheck(white)) return false;
                        }
                    }
                }
            }
        }
    }
    return true;
}
