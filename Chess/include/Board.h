#pragma once

#include "Pieces/Piece.h"
#include <memory>
#include <vector>
#include "Utils/CMove.h"
#include "Utils/MoveScorePair.h"

class Board {
private:
    std::vector<std::vector<std::unique_ptr<Piece>>> grid;

public:
    Board();
    Board(const Board& other);
    Board& operator=(const Board& rhs);

    // AI support functions
    std::vector<CMove> generateLegalMoves(bool whiteToMove) const;
    void applyMove(CMove m);
    bool inCheck(bool whiteKing) const;
    
    // Add this missing declaration:
    bool isSquareAttackedByOpponent(int row, int col, bool opponentIsWhite) const;
    bool isValidCastlingMove(int srcRow, int srcCol, int destRow, int destCol, bool isWhite) const;

    Piece* getPiece(int row, int col) const;
    void setPiece(int row, int col, std::unique_ptr<Piece> piece);
    std::unique_ptr<Piece> removePiece(int row, int col);
};

namespace AI
{
    using ::MoveScorePair;
    std::vector<MoveScorePair> findBestMoves(const Board& board, bool isWhite, int limit);
}