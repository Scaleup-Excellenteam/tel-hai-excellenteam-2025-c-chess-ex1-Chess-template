#include "Pieces/Knight.h"
#include "Utils/Colors.h" // Include for Unicode symbols
#include "Board.h"

#include <cmath> // for abs()

Knight::Knight(bool isWhite) : Piece(isWhite) {
    this->setSymbol(isWhite ? Colors::Pieces::WHITE_KNIGHT : Colors::Pieces::BLACK_KNIGHT); // Set string symbol
    this->setIsAlive(true);
    this->setIsWhite(isWhite);
}

// Knight's specific legalMoves implementation
std::vector<CMove> Knight::legalMoves(int r, int c, const Board& b) const {
    std::vector<CMove> moves;
    int dr_offsets[] = {-2, -2, -1, -1, 1, 1, 2, 2};
    int dc_offsets[] = {-1, 1, -2, 2, -2, 2, -1, 1};

    for (int i = 0; i < 8; ++i) {
        int destRow = r + dr_offsets[i];
        int destCol = c + dc_offsets[i];

        if (destRow >= 0 && destRow < 8 && destCol >= 0 && destCol < 8) {
            if (isValidMove(r, c, destRow, destCol, b)) { // Use Knight's isValidMove logic
                moves.emplace_back(r, c, destRow, destCol);
            }
        }
    }
    return moves;
}

bool Knight::isValidMove(int srcRow,int srcCol,
                         int destRow,int destCol,
                         const Board& board) const
{
    int dr = std::abs(destRow - srcRow);
    int dc = std::abs(destCol - srcCol);

    if (!((dr == 1 && dc == 2) || (dr == 2 && dc == 1)))
        return false;

    const Piece* dst = board.getPiece(destRow, destCol);
    return dst == nullptr || dst->getIsWhite() != this->getIsWhite();
}