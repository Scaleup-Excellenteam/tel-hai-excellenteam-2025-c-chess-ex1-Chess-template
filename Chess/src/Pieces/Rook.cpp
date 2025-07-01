#include "Pieces/Rook.h"
#include "Utils/Colors.h"
#include "Board.h"
#include <cmath>

Rook::Rook(bool isWhite) : Piece(isWhite), hasMoved_(false) {
    this->setSymbol(isWhite ? Colors::Pieces::WHITE_ROOK : Colors::Pieces::BLACK_ROOK);
    this->setIsAlive(true);
    this->setIsWhite(isWhite);
}

// Proper clone implementation that preserves hasMoved_ state
std::unique_ptr<Piece> Rook::clone() const {
    auto cloned = std::make_unique<Rook>(this->getIsWhite());
    cloned->setHasMoved(this->hasMoved_);
    cloned->setIsAlive(this->getIsAlive());
    return cloned;
}

std::vector<CMove> Rook::legalMoves(int r, int c, const Board& b) const {
    std::vector<CMove> moves;
    int dr[] = {-1, 1, 0, 0};
    int dc[] = {0, 0, -1, 1};

    for (int i = 0; i < 4; ++i) {
        for (int step = 1; step < 8; ++step) {
            int destRow = r + dr[i] * step;
            int destCol = c + dc[i] * step;

            if (destRow < 0 || destRow >= 8 || destCol < 0 || destCol >= 8) {
                break;
            }

            if (isValidMove(r, c, destRow, destCol, b)) {
                moves.emplace_back(r, c, destRow, destCol);
            } else {
                const Piece* destPiece = b.getPiece(destRow, destCol);
                if (destPiece != nullptr && destPiece->getIsWhite() == this->getIsWhite()) {
                    break;
                }
                if (destPiece != nullptr && destPiece->getIsWhite() != this->getIsWhite()) {
                    break;
                }
                if (b.getPiece(destRow, destCol) != nullptr) {
                    break;
                }
            }
        }
    }
    return moves;
}

bool Rook::isValidMove(int srcRow, int srcCol, int destRow, int destCol, const Board& board) const
{
    if (srcRow != destRow && srcCol != destCol)
        return false;

    int stepR = (destRow > srcRow) - (destRow < srcRow);
    int stepC = (destCol > srcCol) - (destCol < srcCol);

    for (int r = srcRow + stepR, c = srcCol + stepC;
         r != destRow || c != destCol;
         r += stepR, c += stepC)
    {
        if (board.getPiece(r, c) != nullptr)
            return false;
    }

    const Piece* dst = board.getPiece(destRow, destCol);
    return dst == nullptr || dst->getIsWhite() != this->getIsWhite();
}