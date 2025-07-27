// Chess/include/Board.h
#pragma once
#include "Pieces/Piece.h"
#include <memory>
#include <vector>
#include "Utils/CMove.h"
#include "Utils/MoveScorePair.h"
#include "Pieces/Pawn.h"


class Board {
private:
    std::vector<std::vector<std::unique_ptr<Piece>>> grid;
    CMove lastMove; // Keep lastMove private

public:
    Board();
    Board(const Board& other);
    Board& operator=(const Board& rhs);

    // AI support functions
    std::vector<CMove> generateLegalMoves(bool whiteToMove) const;
    void applyMove(CMove m);
    bool inCheck(bool whiteKing) const;

    bool isSquareAttackedByOpponent(int row, int col, bool opponentIsWhite) const;
    bool isValidCastlingMove(int srcRow, int srcCol, int destRow, int destCol, bool isWhite) const;

    bool isPawnPromotionReady(int row, int col) const;

    Piece* getPiece(int row, int col) const;
    void setPiece(int row, int col, std::unique_ptr<Piece> piece);
    std::unique_ptr<Piece> removePiece(int row, int col);

    CMove getLastMove() const { return lastMove; } // Getter for lastMove

    // ADDED: Setter for lastMove for testing purposes
    void setLastMoveForTest(CMove move) { lastMove = move; }
};

namespace AI
{
    using ::MoveScorePair;
    std::vector<MoveScorePair> findBestMoves(const Board& board, bool isWhite, int limit, int searchDepth);
    std::vector<MoveScorePair> findBestMoves(const Board& board, bool isWhite, int limit, int threadCount, int searchDepth);
}