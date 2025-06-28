#include "Pieces/King.h"
#include "Utils/Colors.h" // Include for Unicode symbols
#include "Board.h"
#include "Pieces/Pawn.h" // Needed for dynamic_cast<Pawn*>
#include <cmath> // for abs()


King::King(bool isWhite) : Piece(isWhite) {
    this->setSymbol(isWhite ? Colors::Pieces::WHITE_KING : Colors::Pieces::BLACK_KING); // Set string symbol
    this->setIsAlive(true);
    this->setIsWhite(isWhite);
}

// King's specific legalMoves implementation
std::vector<CMove> King::legalMoves(int r, int c, const Board& b) const {
    std::vector<CMove> moves;
    for (int dr_offset = -1; dr_offset <= 1; ++dr_offset) {
        for (int dc_offset = -1; dc_offset <= 1; ++dc_offset) {
            if (dr_offset == 0 && dc_offset == 0) {
                continue; // King doesn't move to its own square
            }
            int destRow = r + dr_offset;
            int destCol = c + dc_offset;

            // Check if destination is within bounds (0-7)
            if (destRow >= 0 && destRow < 8 && destCol >= 0 && destCol < 8) {
                if (isValidMove(r, c, destRow, destCol, b)) { // Use King's isValidMove logic
                    moves.emplace_back(r, c, destRow, destCol);
                }
            }
        }
    }
    // TODO: Add Castling moves here in a later phase.
    return moves;
}

bool King::isValidMove(int srcRow,
                       int srcCol,
                       int destRow,
                       int destCol,
                       const Board& board) const
{
    // A king moves at most one square in any direction
    if (std::abs(srcRow - destRow) <= 1 && std::abs(srcCol - destCol) <= 1)
    {
        Piece* destPiece = board.getPiece(destRow, destCol);

        // Legal if the destination is empty or holds an opponent piece
        return destPiece == nullptr ||
               destPiece->getIsWhite() != this->getIsWhite();
    }

    return false; // Any other displacement is illegal for a king
}

bool King::isInCheck(int row, int col, const Board& board) const {
    // This method is correctly implemented here to check if a specific king at (row, col) is attacked.
    for (int r = 0; r < 8; ++r) {
        for (int c = 0; c < 8; ++c) {
            Piece* piece = board.getPiece(r, c);
            // Ensure the piece exists, is an opponent's, and can attack the king's square
            if (piece != nullptr && piece->getIsWhite() != this->getIsWhite()) {
                // Pawns have special capture rules for checking
                if (Pawn* pawn = dynamic_cast<Pawn*>(piece)) {
                    if (pawn->isValidCapture(r, c, row, col, board)) return true;
                } else {
                    if (piece->isValidMove(r, c, row, col, board)) return true;
                }
            }
        }
    }
    return false; // King is not in check
}

// These methods are indeed redundant for the overall game state
// as GameEngine uses Board::generateLegalMoves and Board::inCheck.
// They are kept here for now but will be slated for removal/cleanup.
bool King::isInCheckmate(int row, int col, const Board& board) const {
    // This function will be removed or made private if needed for very specific King checks
    // The main checkmate logic is in GameEngine using board->generateLegalMoves()
    return false; // Placeholder
}

bool King::isInStalemate(int row, int col, const Board& board) const {
    // This function will be removed or made private
    // The main stalemate logic is in GameEngine using board->generateLegalMoves()
    return false; // Placeholder
}