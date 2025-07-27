// Chess/include/GameEngine.h
#pragma once
#include <string>
#include <memory>
#include <vector>
#include <iostream>
#include <map>
#include <sstream> // For std::istringstream in tests

class Board;
class Piece;

#include "Board.h"
#include "Pieces/Piece.h"
#include "Pieces/Rook.h"
#include "Pieces/King.h"
#include "Pieces/Queen.h"
#include "Pieces/Bishop.h"
#include "Pieces/Pawn.h"
#include "Pieces/Knight.h"
#include "AI/BestMoveFinder.h"

class Chess;

class GameEngine {
private:
    int m_searchDepth;

    void displayWelcomeBanner() const;
    void getUserSettings();
    void runInteractiveGame();
    void runBenchmark(int searchDepth);

    void handlePawnPromotion(int row, int col);

    std::unique_ptr<Board> board;
    bool isWhiteTurn_;
    int m_codeResponse;
    bool m_playAgainstAI; // Keep private

    int m_halfMoveClock;
    std::map<std::string, int> m_boardHistory;

    void putPiece(int row, int col, Piece* raw);
    std::string getBoardStateString() const;

public:
    GameEngine();

    void run();

    void initGame();
    void setCodeResponse(int code);
    int getCodeResponse() const;
    bool isCheck() const;

    void makeMove(const std::string& moveStr);
    bool makeMove(int srcRow, int srcCol, int destRow, int destCol);

    int validateMove(const std::string &mv) const;

    const Board &currentBoard() const { return *board; }

    bool isCheckmate() const;
    bool isStalemate() const;
    bool isInsufficientMaterial() const;
    bool isFiftyMoveDraw() const;
    bool isThreefoldRepetition() const;

    bool whiteToMove() const { return isWhiteTurn_; }
    int getSearchDepth() const { return m_searchDepth; }


    // --- PUBLIC METHODS FOR TESTING ONLY ---
    void resetGameForTest();
    void setPieceForTest(int row, int col, Piece* rawPiece);
    void setTurnForTest(bool isWhite);
    void setHalfMoveClockForTest(int count);
    void clearBoardHistoryForTest();
    void addBoardStateToHistoryForTest();

    // ADDED: Setter for m_playAgainstAI for tests
    void setPlayAgainstAIForTest(bool value); // THIS LINE WAS MISSING

    // Setter for Board's lastMove for tests
    void setLastMoveForTest(CMove move);
};