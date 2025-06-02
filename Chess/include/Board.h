#pragma once

#include <vector>
#include <string>
#include <mutex>
#include <atomic>
#include <utility>
#include "PriorityQueue.h"
#include "Piece.h"

// Forward‐declare ThreadPool so we can use a pointer here
class ThreadPool;

class Board {
private:
    // 8×8 grid of Piece* (nullptr if empty)
    std::vector<std::vector<Piece*>> board_game;

    // 64‐char string, row0 = rank 8, row7 = rank 1
    std::string& sharedBoardString;

    // Which side to move: true = White, false = Black
    bool whiteTurn;

    // Multithreading synchronization:
    std::mutex pqMutex;              // protects bestMoves
    std::mutex restoreMutex;         // protects board restoration
    std::atomic<bool> stopFlag{ false };
    int scoreThreshold = INT_MAX;    // early‐stop threshold (optional)

    static const int MAX_QUEUE_SIZE = 5;
    PriorityQueue<std::pair<std::string,int>> bestMoves;

    // Helper: check if straight‐line path is clear (no pieces between src and dst)
    [[nodiscard]] bool isPathClear(int r1, int c1, int r2, int c2) const;

    // Evaluation function (existing code)
    [[nodiscard]] int scorePosition(const std::string&) const;

    // Minimax search (existing code)
    int minimax(int depth);

    // Re‐populate board_game from sharedBoardString
    void rebuildBoard();

    // Factory: create a Piece‐subclass based on the character
    static Piece* createPiece(char type);

public:
    explicit Board(std::string& boardStr);
    ~Board();
    std::string getRawString() const {
        return sharedBoardString;
    }

    // Public getter so main.cpp can call logic.isWhiteTurn()
    bool isWhiteTurn() const { return whiteTurn; }

    // Recommend up to topN moves at depth maxDepth.
    // If pool==nullptr → single‐threaded; otherwise dispatches parallel jobs.
    std::vector<std::pair<std::string,int>>
    recommendMoves(int maxDepth = 2, int topN = 3, ThreadPool* pool = nullptr);

    // Attempt to move “e2e4”, set responseCode=0 if success, nonzero if illegal.
    // Returns true if the move was executed (updating board_game & sharedBoardString),
    // false if illegal (nothing changed).
    bool movePiece(const std::string& input, int& responseCode);

    // ASCII‐art print ('.' for empty, piece‐char for occupied)
    [[nodiscard]] std::string toString() const;
    friend std::ostream& operator<<(std::ostream& os, Board& board);
};
