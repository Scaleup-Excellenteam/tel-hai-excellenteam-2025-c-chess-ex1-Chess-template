#include "Pieces/King.h"
#include "Pieces/Pawn.h"
#include "Board.h"
#include "Utils/Colors.h"
#include <cmath>
#include "Pieces/Rook.h" // dynamic_cast to Rook


King::King(bool isWhite) : Piece(isWhite), hasMoved_(false) {
    this->setSymbol(isWhite ? Colors::Pieces::WHITE_KING : Colors::Pieces::BLACK_KING); // Use Unicode symbols
    this->setIsAlive(true);
    this->setIsWhite(isWhite);
}

// Update clone to copy hasMoved_ state
std::unique_ptr<Piece> King::clone() const {
    auto cloned = std::make_unique<King>(this->getIsWhite());
    cloned->setHasMoved(this->hasMoved_); // Copy the hasMoved_ state
    cloned->setIsAlive(this->getIsAlive());
    return cloned;
}


bool King::isValidMove(int srcRow, int srcCol, int destRow, int destCol, const Board& board) const
{
    // Check for castling first (king moves 2 squares horizontally)
    if (abs(destCol - srcCol) == 2 && srcRow == destRow) {
        return canCastle(srcRow, srcCol, destRow, destCol, board);
    }

    // Normal king movement (one square in any direction)
    if (abs(srcRow - destRow) <= 1 && abs(srcCol - destCol) <= 1) {
        Piece* destPiece = board.getPiece(destRow, destCol);
        return destPiece == nullptr || destPiece->getIsWhite() != this->getIsWhite();
    }

    return false;
}

bool King::canCastle(int srcRow, int srcCol, int destRow, int destCol, const Board& board) const {
    // Determine if this is kingside (short) or queenside (long) castling
    bool isKingside = (destCol > srcCol);
    int rookCol = isKingside ? 7 : 0;

    // Check if king is in starting position AND has not moved
    int expectedKingRow = this->getIsWhite() ? 7 : 0;
    int expectedKingCol = 4;
    if (srcRow != expectedKingRow || srcCol != expectedKingCol || this->getHasMoved()) {
        return false;
    }

    // Check if rook exists and is in starting position AND has not moved
    Piece* rook = board.getPiece(srcRow, rookCol);
    if (!rook || rook->getIsWhite() != this->getIsWhite() ||
        (rook->getSymbol() != Colors::Pieces::WHITE_ROOK && rook->getSymbol() != Colors::Pieces::BLACK_ROOK)) {
        return false;
    }
    if (Rook* r_piece = dynamic_cast<Rook*>(rook)) {
        if (r_piece->getHasMoved()) {
            return false;
        }
    } else { // It's not a Rook or dynamic_cast failed (e.g., nullptr or wrong type)
        return false;
    }

    // Check if king is currently in check
    if (isInCheck(srcRow, srcCol, board)) {
        return false;
    }

    // Move these declarations outside the 'if' block that might prevent their declaration
    int start = std::min(srcCol, rookCol);
    int end = std::max(srcCol, rookCol);

    // Check if path is clear between king and rook
    for (int c = start + 1; c < end; c++) {
        if (board.getPiece(srcRow, c) != nullptr) {
            return false;
        }
    }

    // Check if king passes through or ends in check during castling
    int direction = isKingside ? 1 : -1;
    for (int step = 1; step <= 2; step++) {
        int checkCol = srcCol + (step * direction);
        if (wouldBeInCheck(srcRow, checkCol, board)) {
            return false;
        }
    }

    return true;
}

bool King::wouldBeInCheck(int row, int col, const Board& board) const {
    for (int r = 0; r < 8; ++r) {
        for (int c = 0; c < 8; ++c) {
            Piece* piece = board.getPiece(r, c);
            if (piece != nullptr && piece->getIsWhite() != this->getIsWhite()) {
                if (Pawn* pawn = dynamic_cast<Pawn*>(piece)) {
                    if (pawn->isValidCapture(r, c, row, col, board)) {
                        return true;
                    }
                } else if (piece->isValidMove(r, c, row, col, board)) {
                    return true;
                }
            }
        }
    }
    return false;
}

bool King::isInCheck(int row, int col, const Board& board) const {
    for (int r = 0; r < 8; ++r) {
        for (int c = 0; c < 8; ++c) {
            Piece* piece = board.getPiece(r, c);
            if (piece != nullptr && piece->getIsWhite() != this->getIsWhite() && piece->isValidMove(r, c, row, col, board)) {
                return true;
            }
        }
    }
    return false;
}

bool King::isInCheckmate(int row, int col, const Board& board) const {
    if (!isInCheck(row, col, board)) {
        return false;
    }

    for (int r = row - 1; r <= row + 1; ++r) {
        for (int c = col - 1; c <= col + 1; ++c) {
            if (r >= 0 && r < 8 && c >= 0 && c < 8) {
                if (isValidMove(row, col, r, c, board) && !isInCheck(r, c, board)) {
                    return false;
                }
            }
        }
    }
    return true;
}

bool King::isInStalemate(int row, int col, const Board& board) const {
    if (isInCheck(row, col, board)) {
        return false;
    }

    for (int r = row - 1; r <= row + 1; ++r) {
        for (int c = col - 1; c <= col + 1; ++c) {
            if (r >= 0 && r < 8 && c >= 0 && c < 8) {
                if (isValidMove(row, col, r, c, board) && !isInCheck(r, c, board)) {
                    return false;
                }
            }
        }
    }
    return true;
}