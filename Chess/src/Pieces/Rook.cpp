#include "Pieces/Rook.h"
#include "Utils/Colors.h" // Include for Unicode symbols
#include "Board.h"
#include <cmath> // for abs()


Rook::Rook(bool isWhite) : Piece(isWhite) {
    this->setSymbol(isWhite ? Colors::Pieces::WHITE_ROOK : Colors::Pieces::BLACK_ROOK); // Set string symbol
    this->setIsAlive(true);
    this->setIsWhite(isWhite);
}

// Rook's specific legalMoves implementation
std::vector<CMove> Rook::legalMoves(int r, int c, const Board& b) const {
    std::vector<CMove> moves;
    // Check all 4 straight directions
    int dr[] = {-1, 1, 0, 0};
    int dc[] = {0, 0, -1, 1};

    for (int i = 0; i < 4; ++i) {
        for (int step = 1; step < 8; ++step) {
            int destRow = r + dr[i] * step;
            int destCol = c + dc[i] * step;

            if (destRow < 0 || destRow >= 8 || destCol < 0 || destCol >= 8) {
                break; // Out of bounds
            }

            if (isValidMove(r, c, destRow, destCol, b)) { // Use existing isValidMove logic
                moves.emplace_back(r, c, destRow, destCol);
            } else {
                // If the move itself is invalid due to an obstacle, stop checking in this direction
                const Piece* destPiece = b.getPiece(destRow, destCol);
                if (destPiece != nullptr && destPiece->getIsWhite() == this->getIsWhite()) {
                    break; // Blocked by own piece
                }
                if (destPiece != nullptr && destPiece->getIsWhite() != this->getIsWhite()) {
                    // Captured opponent piece, but path beyond is blocked.
                    // isValidMove would have returned true for this capture, but now we break.
                    break;
                }
                // If we reach here, it means isValidMove returned false likely due to an obstacle
                // or the move being illegal for the piece, so we stop checking further in this direction.
                if (b.getPiece(destRow, destCol) != nullptr) { // Path blocked by ANY piece
                    break;
                }
            }
        }
    }
    return moves;
}


bool Rook::isValidMove(int srcRow,
                       int srcCol,
                       int destRow,
                       int destCol,
                       const Board& board) const
{
    /* Must stay in same row or same column */
    if (srcRow != destRow && srcCol != destCol)
        return false;

    /* Step direction */
    int stepR = (destRow > srcRow) - (destRow < srcRow);  // 1,-1,0
    int stepC = (destCol > srcCol) - (destCol < srcCol);

    /* Path must be clear (exclude destination square) */
    for (int r = srcRow + stepR, c = srcCol + stepC;
         r != destRow || c != destCol;
         r += stepR,    c += stepC)
    {
        if (board.getPiece(r, c) != nullptr)
            return false;
    }

    /* Destination either empty or opponent piece */
    const Piece* dst = board.getPiece(destRow, destCol);
    return dst == nullptr || dst->getIsWhite() != this->getIsWhite();
}