#include "Pieces/Bishop.h"
#include "Utils/Colors.h" // Include for Unicode symbols
#include "Board.h"
#include <cmath> // for abs()

Bishop::Bishop(bool isWhite) : Piece(isWhite) {
    this->setSymbol(isWhite ? Colors::Pieces::WHITE_BISHOP : Colors::Pieces::BLACK_BISHOP); // Set string symbol
    this->setIsAlive(true);
    this->setIsWhite(isWhite);
}

// Implement legalMoves for Bishop more efficiently than the generic Piece one
std::vector<CMove> Bishop::legalMoves(int r, int c, const Board& b) const {
    std::vector<CMove> moves;
    // Check all 4 diagonal directions
    int dr[] = {-1, -1, 1, 1};
    int dc[] = {-1, 1, -1, 1};

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
                // This assumes isValidMove returns false for obstacles and own pieces beyond first square
                const Piece* destPiece = b.getPiece(destRow, destCol);
                if (destPiece != nullptr && destPiece->getIsWhite() == this->getIsWhite()) {
                    break; // Blocked by own piece
                }
                if (destPiece != nullptr && destPiece->getIsWhite() != this->getIsWhite()) {
                    // Captured opponent piece, but path beyond is blocked.
                    // isValidMove would have returned true for this capture, but now we break.
                    break;
                }
                // If it's blocked by a piece of the same color, or if a path is illegal, break
                if (std::abs(destRow - r) != std::abs(destCol - c) || (step > 1 && b.getPiece(destRow - dr[i], destCol - dc[i]) != nullptr && b.getPiece(destRow - dr[i], destCol - dc[i])->getIsWhite() == this->getIsWhite())) {
                    break;
                }
                // If we reach here, it means isValidMove returned false likely due to an obstacle
                // or the move being illegal for the piece, so we stop checking further in this direction.
                if (b.getPiece(destRow, destCol) != nullptr) { // Path blocked by opponent piece
                    break;
                }
            }
        }
    }
    return moves;
}

bool Bishop::isValidMove(int srcRow,int srcCol,
                         int destRow,int destCol,
                         const Board& board) const
{
    if (std::abs(destRow - srcRow) != std::abs(destCol - srcCol))
        return false;                           // must stay on a diagonal

    int stepR = (destRow > srcRow) ? 1 : -1;
    int stepC = (destCol > srcCol) ? 1 : -1;

    for (int r = srcRow + stepR, c = srcCol + stepC;
         r != destRow;
         r += stepR, c += stepC)
        if (board.getPiece(r, c) != nullptr)
            return false;

    const Piece* dst = board.getPiece(destRow, destCol);
    return dst == nullptr || dst->getIsWhite() != this->getIsWhite();
}