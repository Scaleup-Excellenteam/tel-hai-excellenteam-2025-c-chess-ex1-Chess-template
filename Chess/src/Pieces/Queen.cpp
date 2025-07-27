#include "Pieces/Queen.h"
#include "Utils/Colors.h" // Include for Unicode symbols
#include "Board.h"
#include "Pieces/Rook.h" // To use Rook's isValidMove logic
#include "Pieces/Bishop.h" // To use Bishop's isValidMove logic
#include <cmath> // for abs()

Queen::Queen(bool isWhite) : Piece(isWhite) {
    this->setSymbol(isWhite ? Colors::Pieces::WHITE_QUEEN : Colors::Pieces::BLACK_QUEEN); // Set string symbol
    this->setIsAlive(true);
    this->setIsWhite(isWhite);
}

// Queen's specific legalMoves implementation (combines Rook and Bishop)
std::vector<CMove> Queen::legalMoves(int r, int c, const Board& b) const {
    std::vector<CMove> moves;
    // Generate moves as a Rook
    Rook tempRook(this->getIsWhite());
    std::vector<CMove> rookMoves = tempRook.legalMoves(r, c, b);
    moves.insert(moves.end(), rookMoves.begin(), rookMoves.end());

    // Generate moves as a Bishop
    Bishop tempBishop(this->getIsWhite());
    std::vector<CMove> bishopMoves = tempBishop.legalMoves(r, c, b);
    moves.insert(moves.end(), bishopMoves.begin(), bishopMoves.end());

    return moves;
}

bool Queen::isValidMove(int srcRow,int srcCol,
                        int destRow,int destCol,
                        const Board& board) const
{
    const bool is_white = this->getIsWhite();

    if (srcRow == destRow || srcCol == destCol) // rook-like movement
    {
        return Rook(is_white).isValidMove(srcRow, srcCol, destRow, destCol, board);
    }

    if (std::abs(destRow - srcRow) == std::abs(destCol - srcCol)) // bishop-like movement
    {
        return Bishop(is_white).isValidMove(srcRow, srcCol, destRow, destCol, board);
    }

    return false;
}