// src/Board.cpp
#include "Board.h"
#include "ThreadPool.h"

#include "Pawn.h"
#include "Knight.h"
#include "Bishop.h"
#include "Rook.h"
#include "Queen.h"
#include "King.h"

#include <algorithm>
#include <chrono>
#include <cctype>
#include <iostream>

//
// Constructor / Destructor
//
Board::Board(std::string& boardStr)
        : sharedBoardString(boardStr), whiteTurn(true)
{
    board_game.resize(8, std::vector<Piece*>(8, nullptr));
    rebuildBoard();
}

Board::~Board() {
    for (auto &row : board_game) {
        for (auto* p : row) {
            delete p;
        }
    }
}

//
// rebuildBoard() – reconstruct board_game[][] from sharedBoardString (64 chars)
//
void Board::rebuildBoard() {
    // Delete any existing Piece* in board_game
    for (auto &row : board_game) {
        for (auto* p : row) {
            delete p;
        }
    }
    board_game.assign(8, std::vector<Piece*>(8, nullptr));

    // For each character in sharedBoardString, create a Piece if not '#'
    for (int idx = 0; idx < 64; ++idx) {
        char ch = sharedBoardString[idx];
        int r = idx / 8, c = idx % 8;
        if (ch == '#') {
            board_game[r][c] = nullptr;
        } else {
            board_game[r][c] = createPiece(ch);
        }
    }
}

//
// createPiece() – factory to instantiate the correct subclass based on type char
//
Piece* Board::createPiece(char type) {
    switch (type) {
        case 'P': case 'p': return new Pawn(type);
        case 'N': case 'n': return new Knight(type);
        case 'B': case 'b': return new Bishop(type);
        case 'R': case 'r': return new Rook(type);
        case 'Q': case 'q': return new Queen(type);
        case 'K': case 'k': return new King(type);
        default: return nullptr;
    }
}

//
// toString() – ASCII‐art representation of the 8×8 grid ('.' or piece char)
//
std::string Board::toString() const {
    std::string out;
    for (int r = 0; r < 8; ++r) {
        for (int c = 0; c < 8; ++c) {
            if (board_game[r][c]) {
                out.push_back(board_game[r][c]->get_type());
            } else {
                out.push_back('.');
            }
        }
        out.push_back('\n');
    }
    return out;
}

std::ostream& operator<<(std::ostream& os, Board& board) {
    os << board.toString();
    return os;
}

//
// movePiece(input, responseCode):
//   - Delegates legality check to the piece itself via is_legel_movement(...).
//   - If legal, performs the capture/move on both board_game[][] and sharedBoardString.
//   - Flips whiteTurn and returns true. Otherwise returns false with an error code.
//
bool Board::movePiece(const std::string& input, int& responseCode) {
    // 1) Input must be exactly 4 chars, e.g. "e2e4"
    if (input.size() != 4) {
        responseCode = 1; // illegal format
        return false;
    }
    char fcol = std::tolower(input[0]);
    char frank = input[1];
    char tcol = std::tolower(input[2]);
    char trank = input[3];

    // 2) Check within 'a'..'h' and '1'..'8'
    if (fcol < 'a' || fcol > 'h' || tcol < 'a' || tcol > 'h' ||
        frank < '1' || frank > '8' || trank < '1' || trank > '8') {
        responseCode = 2; // out‐of‐bounds
        return false;
    }
    int srcCol = fcol - 'a';
    int srcRow = '8' - frank;  // e.g. '2' → 6
    int dstCol = tcol - 'a';
    int dstRow = '8' - trank;  // e.g. '4' → 4

    // 3) Must be a piece at source
    Piece* psrc = board_game[srcRow][srcCol];
    if (!psrc) {
        responseCode = 3; // no piece to move
        return false;
    }

    // 4) Must move the correct color
    bool pieceIsWhite = std::isupper(psrc->get_type());
    if (pieceIsWhite != whiteTurn) {
        responseCode = 4; // wrong side to move
        return false;
    }

    // 5) Ask the piece if this raw move is legal, given the entire 64‐char board string
    //    (Note: we pass sharedBoardString so Pawn can examine en passant or promotion if implemented)
    if (!psrc->is_legel_movement(input, sharedBoardString, whiteTurn)) {
        responseCode = 7; // piece-specific movement illegal
        return false;
    }

    // 6) Cannot capture your own piece
    Piece* pdst = board_game[dstRow][dstCol];
    if (pdst) {
        bool destIsWhite = std::isupper(pdst->get_type());
        if (destIsWhite == pieceIsWhite) {
            responseCode = 5; // cannot capture own piece
            return false;
        }
    }

    // 7) Execute the move:
    //    - Delete captured piece if any
    if (pdst) delete pdst;
    board_game[dstRow][dstCol] = psrc;
    board_game[srcRow][srcCol] = nullptr;

    //    - Update sharedBoardString (64 chars).
    int srcIndex = srcRow * 8 + srcCol;
    int dstIndex = dstRow * 8 + dstCol;
    sharedBoardString[srcIndex] = '#';
    sharedBoardString[dstIndex] = psrc->get_type();

    // 8) Flip side to move
    whiteTurn = !whiteTurn;
    responseCode = 0; // OK
    return true;
}

//
// minimax(depth)
//   (Your existing minimax implementation belongs here, unchanged.)
//
int Board::minimax(int depth) {
    // … (your existing code) …
    return 0;
}

//
// scorePosition(boardStr)
//   (Your existing evaluation function belongs here, unchanged.)
//
int Board::scorePosition(const std::string& boardStr) const {
    // … (your existing code) …
    return 0;
}

//
// recommendMoves(maxDepth, topN, pool)
//   (All of your previous threaded move‐generation logic goes here;
//    it calls movePiece(...) above, which now delegates legality to each Piece.)
//
std::vector<std::pair<std::string,int>>
Board::recommendMoves(int maxDepth, int topN, ThreadPool* pool) {
    // 1) Clear any old bestMoves
    {
        std::lock_guard<std::mutex> lock(pqMutex);
        bestMoves.clear();
    }
    stopFlag.store(false);

    // 2) Save original 64‐char string and whose turn it is
    std::string oldBoard = sharedBoardString;
    bool oldTurn = whiteTurn;

    // 3) Gather every candidate (srcRow,srcCol) → (dstRow,dstCol)
    std::vector<std::tuple<int,int,int,int>> allCombos;
    for (int r = 0; r < 8; ++r) {
        for (int c = 0; c < 8; ++c) {
            Piece* p = board_game[r][c];
            if (!p) continue;
            bool pIsWhite = std::isupper(p->get_type());
            if (pIsWhite != whiteTurn) continue;

            for (int rr = 0; rr < 8; ++rr) {
                for (int cc = 0; cc < 8; ++cc) {
                    if (r == rr && c == cc) continue;
                    allCombos.emplace_back(r, c, rr, cc);
                }
            }
        }
    }
    int totalJobs = static_cast<int>(allCombos.size());
    std::atomic<int> jobsRemaining(totalJobs);

    // 4) If pool == nullptr → run single‐threaded
    if (pool == nullptr) {
        for (auto &tup : allCombos) {
            if (stopFlag.load(std::memory_order_relaxed)) break;

            int r, c, rr, cc;
            std::tie(r, c, rr, cc) = tup;
            char sr = static_cast<char>('a' + c);
            char sf = static_cast<char>('1' + (7 - r));
            char dr = static_cast<char>('a' + cc);
            char df = static_cast<char>('1' + (7 - rr));
            std::string mv{ sr, sf, dr, df };

            int code;
            bool moved = this->movePiece(mv, code);
            if (moved) {
                int sc = this->minimax(maxDepth - 1);
                if (sc >= scoreThreshold) {
                    {
                        std::lock_guard<std::mutex> lock(pqMutex);
                        bestMoves.push({ mv, sc });
                        if (bestMoves.size() > MAX_QUEUE_SIZE)
                            bestMoves.popWorst();
                    }
                    stopFlag.store(true, std::memory_order_relaxed);
                    break;
                }
                {
                    std::lock_guard<std::mutex> lock(pqMutex);
                    bestMoves.push({ mv, sc });
                    if (bestMoves.size() > MAX_QUEUE_SIZE)
                        bestMoves.popWorst();
                }
            }
            // Restore board for next candidate
            sharedBoardString = oldBoard;
            whiteTurn = oldTurn;
            this->rebuildBoard();
        }
    }
        // 5) Otherwise → multithreaded using ThreadPool
    else {
        for (auto &tup : allCombos) {
            pool->submit([this, tup, &oldBoard, oldTurn, &jobsRemaining, maxDepth] {
                if (stopFlag.load(std::memory_order_relaxed)) {
                    jobsRemaining.fetch_sub(1, std::memory_order_relaxed);
                    return;
                }
                int r, c, rr, cc;
                std::tie(r, c, rr, cc) = tup;
                char sr = static_cast<char>('a' + c);
                char sf = static_cast<char>('1' + (7 - r));
                char dr = static_cast<char>('a' + cc);
                char df = static_cast<char>('1' + (7 - rr));
                std::string mv{ sr, sf, dr, df };

                int code;
                bool moved;
                {
                    // Protect movePiece + board restoration from races
                    std::lock_guard<std::mutex> lock(restoreMutex);
                    moved = this->movePiece(mv, code);
                }
                if (moved) {
                    int sc = this->minimax(maxDepth - 1);
                    if (sc >= scoreThreshold) {
                        {
                            std::lock_guard<std::mutex> lock(pqMutex);
                            this->bestMoves.push({ mv, sc });
                            if (this->bestMoves.size() > MAX_QUEUE_SIZE)
                                this->bestMoves.popWorst();
                        }
                        stopFlag.store(true, std::memory_order_relaxed);
                        {
                            std::lock_guard<std::mutex> rlock(restoreMutex);
                            this->sharedBoardString = oldBoard;
                            this->whiteTurn = oldTurn;
                            this->rebuildBoard();
                        }
                        jobsRemaining.fetch_sub(1, std::memory_order_relaxed);
                        return;
                    }
                    {
                        std::lock_guard<std::mutex> lock(pqMutex);
                        this->bestMoves.push({ mv, sc });
                        if (this->bestMoves.size() > MAX_QUEUE_SIZE)
                            this->bestMoves.popWorst();
                    }
                }
                {
                    // Restore board
                    std::lock_guard<std::mutex> lock(restoreMutex);
                    this->sharedBoardString = oldBoard;
                    this->whiteTurn = oldTurn;
                    this->rebuildBoard();
                }
                jobsRemaining.fetch_sub(1, std::memory_order_relaxed);
            });
        }
        // Wait for all jobs or early‐stop
        while (jobsRemaining.load(std::memory_order_relaxed) > 0) {
            std::this_thread::yield();
        }
    }

    // 6) Collect up to topN results from bestMoves
    std::vector<std::pair<std::string,int>> all;
    {
        std::lock_guard<std::mutex> lock(pqMutex);
        while (!bestMoves.empty()) {
            all.emplace_back(bestMoves.pull());
            bestMoves.poll();
        }
    }
    std::reverse(all.begin(), all.end());
    std::vector<std::pair<std::string,int>> out;
    for (int i = 0; i < topN && i < static_cast<int>(all.size()); ++i) {
        out.push_back(all[i]);
    }
    return out;
}
