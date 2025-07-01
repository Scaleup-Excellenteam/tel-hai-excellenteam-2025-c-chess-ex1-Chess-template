#include "Board.h"
#include "Pieces/Pawn.h"
#include "Pieces/Bishop.h"
#include "Pieces/King.h"
#include "Pieces/Knight.h"
#include "Pieces/Queen.h"
#include "Pieces/Rook.h"
#include "Utils/Colors.h"
#include <algorithm> // For std::min, std::max

Board::Board() {
    grid.reserve(8);
    for (int i = 0; i < 8; ++i) {
        grid.emplace_back(8);
    }
}

Board::Board(const Board& other) {
    grid.reserve(8);
    for (int r = 0; r < 8; ++r)
        grid.emplace_back(8);

    for (int r = 0; r < 8; ++r) {
        for (int c = 0; c < 8; ++c) {
            Piece* p = other.getPiece(r, c);
            if (p)
                grid[r][c] = p->clone();
        }
    }
}

Board& Board::operator=(const Board& rhs) {
    if (this != &rhs) {
        grid.clear();
        grid.reserve(8);
        for (int r = 0; r < 8; ++r)
            grid.emplace_back(8);

        for (int r = 0; r < 8; ++r) {
            for (int c = 0; c < 8; ++c) {
                Piece* p = rhs.getPiece(r, c);
                if (p)
                    grid[r][c] = p->clone();
            }
        }
    }
    return *this;
}

bool Board::isSquareAttackedByOpponent(int row, int col, bool opponentIsWhite) const {
    for (int r = 0; r < 8; ++r) {
        for (int c = 0; c < 8; ++c) {
            Piece* p = getPiece(r, c);
            if (p && p->getIsWhite() == opponentIsWhite) {
                if (Pawn* pawn = dynamic_cast<Pawn*>(p)) {
                    if (pawn->isValidCapture(r, c, row, col, *this)) return true;
                } else if (p->isValidMove(r, c, row, col, *this)) {
                    return true;
                }
            }
        }
    }
    return false;
}

bool Board::isValidCastlingMove(int srcRow, int srcCol, int destRow, int destCol, bool isWhite) const {
    // Delegate to King's castling logic for backward compatibility
    Piece* piece = getPiece(srcRow, srcCol);
    if (King* king = dynamic_cast<King*>(piece)) {
        return king->isValidMove(srcRow, srcCol, destRow, destCol, *this);
    }
    return false;
}

std::vector<CMove> Board::generateLegalMoves(bool whiteToMove) const {
    std::vector<CMove> legalMoves;

    for (int r = 0; r < 8; ++r) {
        for (int c = 0; c < 8; ++c) {
            Piece* p = getPiece(r, c);
            if (!p || p->getIsWhite() != whiteToMove)
                continue;

            std::vector<CMove> pseudoLegalMoves = p->legalMoves(r, c, *this);

            for (const auto& move : pseudoLegalMoves) {
                Board temp_board = *this;
                temp_board.applyMove(move);

                if (!temp_board.inCheck(whiteToMove)) {
                    legalMoves.push_back(move);
                }
            }
        }
    }

    return legalMoves;
}

void Board::applyMove(CMove m) {
    std::unique_ptr<Piece> moving_piece_owner = removePiece(m.srcRow, m.srcCol);

    if (King* k = dynamic_cast<King*>(moving_piece_owner.get())) {
        if (std::abs(m.destCol - m.srcCol) == 2) {
            int rookSrcCol;
            int rookDestCol;
            if (m.destCol > m.srcCol) {
                rookSrcCol = 7;
                rookDestCol = m.srcCol + 1;
            } else {
                rookSrcCol = 0;
                rookDestCol = m.srcCol - 1;
            }

            std::unique_ptr<Piece> rook_to_move = removePiece(m.srcRow, rookSrcCol);
            if (rook_to_move) {
                if (Rook* r_ptr = dynamic_cast<Rook*>(rook_to_move.get())) {
                    r_ptr->setHasMoved(true);
                }
                setPiece(m.srcRow, rookDestCol, std::move(rook_to_move));
            }
        }
    }

    setPiece(m.destRow, m.destCol, std::move(moving_piece_owner));

    // TODO: Handle Pawn Promotion, En Passant in later phases.
}


bool Board::inCheck(bool whiteKing) const {
    int kingR = -1, kingC = -1;
    const std::string& kingSymbol = whiteKing ? Colors::Pieces::WHITE_KING : Colors::Pieces::BLACK_KING;

    for (int r = 0; r < 8 && kingR < 0; ++r) {
        for (int c = 0; c < 8; ++c) {
            Piece* p = getPiece(r, c);
            if (p && p->getSymbol() == kingSymbol) {
                kingR = r; kingC = c;
                break;
            }
        }
    }
    if (kingR < 0)
        return false;

    for (int r = 0; r < 8; ++r) {
        for (int c = 0; c < 8; ++c) {
            Piece* p = getPiece(r, c);
            if (p && p->getIsWhite() != whiteKing) {
                if (Pawn* pawn = dynamic_cast<Pawn*>(p)) {
                    if (pawn->isValidCapture(r, c, kingR, kingC, *this))
                        return true;
                } else if (p->isValidMove(r, c, kingR, kingC, *this)) {
                    return true;
                }
            }
        }
    }
    return false;
}

Piece* Board::getPiece(int row, int col) const {
    return grid[row][col].get();
}

void Board::setPiece(int row, int col, std::unique_ptr<Piece> piece) {
    grid[row][col] = std::move(piece);
}

std::unique_ptr<Piece> Board::removePiece(int row, int col) {
    std::unique_ptr<Piece> old = std::move(grid[row][col]);
    grid[row][col] = nullptr;
    return old;
}