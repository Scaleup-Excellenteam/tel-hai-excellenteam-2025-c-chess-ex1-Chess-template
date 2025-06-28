#pragma once

#include <string>
#include <memory>
#include <vector>
#include <iostream>

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

    std::unique_ptr<Board> board;
    bool isWhiteTurn_;
    int m_codeResponse;

    void putPiece(int row, int col, Piece* raw);

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

    bool whiteToMove() const { return isWhiteTurn_; }

    int getSearchDepth() const { return m_searchDepth; }
};