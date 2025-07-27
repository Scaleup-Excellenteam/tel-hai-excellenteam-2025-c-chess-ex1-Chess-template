#include "Pieces/Bishop.h"
#include "Utils/Colors.h"
#include "Board.h"
#include <cmath>

Bishop::Bishop(bool isWhite) : Piece(isWhite) {
    this->setSymbol(isWhite ? Colors::Pieces::WHITE_BISHOP : Colors::Pieces::BLACK_BISHOP);
    this->setIsAlive(true);
    this->setIsWhite(isWhite);
}


std::vector<CMove> Bishop::legalMoves(int r, int c, const Board& b) const {
    std::vector<CMove> moves;
    int dr[] = {-1, -1, 1, 1};
    int dc[] = {-1, 1, -1, 1};

    for (int i = 0; i < 4; ++i) {
        for (int step = 1; step < 8; ++step) {
            int destRow = r + dr[i] * step;
            int destCol = c + dc[i] * step;

            if (destRow < 0 || destRow >= 8 || destCol < 0 || destCol >= 8) {
                break; // Out of bounds
            }

            // If the move is valid, add it.
            // If it's invalid, it implies an obstacle or invalid target, so break.
            if (isValidMove(r, c, destRow, destCol, b)) {
                moves.emplace_back(r, c, destRow, destCol);
                // If the destination contains an opponent's piece, it's a capture,
                // and further moves in this direction are blocked.
                if (b.getPiece(destRow, destCol) != nullptr) {
                    break;
                }
            } else {
                // If isValidMove returns false, it's due to an obstacle or invalid target.
                // Stop checking in this direction.
                break;
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
        return false;

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