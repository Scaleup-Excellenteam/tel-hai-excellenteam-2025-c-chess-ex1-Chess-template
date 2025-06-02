#include "Board.h"
#include "Rook.h"
#include "King.h"
#include "Queen.h"
#include "Bishop.h"
#include "Knight.h"
#include "Pawn.h"
#include "Exception_chess.h"
#include <algorithm>
#include <cctype>
#include <thread>
#include <mutex>
#include <atomic>
#include <iostream>
#include <chrono>

Board::Board(std::string& boardStr)
        : sharedBoardString(boardStr), isWhiteTurn(true) {
    board_game.resize(8, std::vector<Piece*>(8, nullptr));
    for (int i = 0; i < 64; ++i) {
        int row = 7 - (i / 8);
        int col = i % 8;
        char pieceChar = boardStr[i];
        board_game[row][col] = createPiece(pieceChar);
    }
}

Board::~Board() {
    for (auto& row : board_game)
        for (auto& piece : row)
            delete piece;
}


Piece* Board::createPiece(char type) {
    if (type == '#') return nullptr;
    switch (tolower(type)) {
        case 'r': return new Rook(type);
        case 'k': return new King(type);
        case 'q': return new Queen(type);
        case 'b': return new Bishop(type);
        case 'n': return new Knight(type);
        case 'p' : return new Pawn(type);
    }
    return nullptr;
}

bool Board::movePiece(const string& input, int& responseCode) {
    int col_source =  input[1] - '1';
    int row_source =  input[0] - 'a';
    int col_dest   =  input[3] - '1';
    int row_dest   =  input[2] - 'a';
    Piece* srcPiece  = board_game[row_source][col_source];
    Piece* destPiece = board_game[row_dest][col_dest];
    try {
        if (!srcPiece)
            throw Exception_chess(ExceptionType::NoPieceAtSource);
        bool isWhitePiece = std::isupper(srcPiece->get_type());
        if (isWhitePiece != isWhiteTurn)
            throw Exception_chess(ExceptionType::OpponentPieceAtSource);
        if (destPiece && (std::isupper(destPiece->get_type()) == isWhiteTurn))
            throw Exception_chess(ExceptionType::FriendlyPieceAtDestination);
        char t = std::tolower(srcPiece->get_type());
        if ((t == 'b' || t == 'r' || t == 'q') &&
            !isPathClear(row_source, col_source, row_dest, col_dest)) {
            throw Exception_chess(ExceptionType::InvalidPieceMovement);
        }
        string flatBoard = toString();
        if (!srcPiece->is_legel_movement(input, flatBoard, isWhiteTurn))
            throw Exception_chess(ExceptionType::InvalidPieceMovement);
        int idx_src  = row_source * 8 + col_source;
        int idx_dest = row_dest   * 8 + col_dest;
        char piece = sharedBoardString[idx_src];
        sharedBoardString[idx_src]  = '#';
        sharedBoardString[idx_dest] = piece;
        board_game[row_dest][col_dest]   = board_game[row_source][col_source];
        board_game[row_source][col_source] = nullptr;
        isWhiteTurn = !isWhiteTurn;
        rebuildBoard();
        responseCode = 42;
        return true;
    } catch (const Exception_chess& ex) {
        switch (ex.getType()) {
            case ExceptionType::NoPieceAtSource:           responseCode = 11; break;
            case ExceptionType::OpponentPieceAtSource:     responseCode = 12; break;
            case ExceptionType::FriendlyPieceAtDestination:responseCode = 13; break;
            case ExceptionType::InvalidPieceMovement:      responseCode = 21; break;
            case ExceptionType::CheckViolation:            responseCode = 31; break;
        }
        return false;
    }
}

std::string Board::toString() const{
    string flat(64, '#');
    for (int r = 0; r < 8; ++r) {
        for (int c = 0; c < 8; ++c) {
            if (board_game[r][c]) {
                flat[(7 - r) * 8 + c] = board_game[r][c]->get_type();
            }
        }
    }
    return flat;
}


bool Board::isPathClear(int rowSrc, int colSrc, int rowDst, int colDst) const {
    if (rowSrc == rowDst) {
        int step = (colDst > colSrc) ? 1 : -1;
        for (int c = colSrc + step; c != colDst; c += step)
            if (board_game[rowSrc][c] != nullptr) return false;
        return true;
    }
    else if (colSrc == colDst) {
        int step = (rowDst > rowSrc) ? 1 : -1;
        for (int r = rowSrc + step; r != rowDst; r += step)
            if (board_game[r][colSrc] != nullptr) return false;
        return true;
    }
    else if (std::abs(rowDst - rowSrc) == std::abs(colDst - colSrc)) {
        int stepR = (rowDst > rowSrc) ? 1 : -1;
        int stepC = (colDst > colSrc) ? 1 : -1;
        int r = rowSrc + stepR, c = colSrc + stepC;
        while (r != rowDst && c != colDst) {
            if (board_game[r][c] != nullptr) return false;
            r += stepR;
            c += stepC;
        }
        return true;
    }
    else {
        return false;
    }
}


void Board::rebuildBoard() {
    for (int r = 0; r < 8; ++r) {
        for (int c = 0; c < 8; ++c) {
            delete board_game[r][c];
            char ch = sharedBoardString[(7 - r) * 8 + c];
            board_game[r][c] = createPiece(ch);
        }
    }
}

int Board::scorePosition(const std::string& flatBoard) const {
    int score = 0;
    for (int i = 0; i < 64; ++i) {
        char P = flatBoard[i];
        if (P=='#') continue;
        bool isWhite = std::isupper(P);
        int pieceValue = 0;
        switch (std::tolower(P)) {
            case 'p': pieceValue=1;  break;
            case 'n': case 'b': pieceValue=3;  break;
            case 'r': pieceValue=5;  break;
            case 'q': pieceValue=9;  break;
            case 'k': pieceValue=100;break;
        }
        score += (isWhite == isWhiteTurn ? +1 : -1) * pieceValue;
    }
    return score;
}

int Board::minimax(int depth) {
    int myScore = scorePosition(sharedBoardString);
    if (depth == 0) return myScore;
    auto next = recommendMoves(depth-1, 1, 1);
    return myScore - ( next.empty() ? 0 : next[0].second );
}


std::vector<std::pair<std::string, int>>
Board::recommendMoves(int maxDepth, int topN, int numThreads) {
    bestMoves.clear();
    std::string oldBoard = sharedBoardString;
    bool oldTurn = isWhiteTurn;
    const int threadsToUse = (numThreads > 0) ? numThreads : std::thread::hardware_concurrency();
    std::vector<std::thread> threads;
    std::atomic<bool> stopFlag = false;
    const int SCORE_THRESHOLD = 90;
    auto worker = [&](int start, int end) {
        for (int idx = start; idx < end && !stopFlag; ++idx) {
            int row = 7 - (idx / 8);
            int col = idx % 8;
            Piece* p = board_game[row][col];
            if (!p) continue;
            char type = p->get_type();
            if (std::isupper(type) != isWhiteTurn) continue;
            char sr = 'a' + col;
            char sf = '1' + (7-row);
            for (int destRow = 0; destRow < 8 && !stopFlag; ++destRow) {
                for (int destCol = 0; destCol < 8 && !stopFlag; ++destCol) {
                    char dr = 'a' + destCol;
                    char df = '1' + (7 - destRow);
                    std::string mv{ sr, sf, dr, df };
                    Board localBoard(sharedBoardString);
                    localBoard.isWhiteTurn = this->isWhiteTurn;
                    int code;
                    if (localBoard.movePiece(mv, code)) {
                        int sc = localBoard.minimax(maxDepth);
                        if (sc >= SCORE_THRESHOLD)
                            stopFlag = true;
                        if (!stopFlag)
                            bestMoves.push({ mv, sc });
                    }
                }
            }
        }
    };
    if (threadsToUse <= 1) {
        worker(0, 64);
    } else {
        int chunk = 64 / threadsToUse + 1;
        for (int i = 0; i < 64; i += chunk) {
            int end = std::min(i + chunk, 64);
            threads.emplace_back(worker, i, end);
        }
        for (auto& t : threads)
            t.join();
    }
    std::vector<std::pair<std::string, int>> out;
    while (!bestMoves.empty() && out.size() < (size_t)topN) {
        out.push_back(bestMoves.poll());
    }

    return out;
}
std::ostream& operator<<(std::ostream& os, Board& board) {
    auto rec = board.recommendMoves(2, 3, 2);
    if (rec.empty()) {
        os << "No recommended moves available.\n";
    } else {
        os << "Recommended moves:";
        for (auto& [mv,sc] : rec) {
            os << " " << mv;
        }
        os << "\n";
    }
    return os;
}

void Board::autoPlayBenchmark(int numThreads, int maxDepth) {
    auto original = sharedBoardString;
    bool originalTurn = isWhiteTurn;
    isWhiteTurn = true;
    auto start = std::chrono::high_resolution_clock::now();
    for (int moveCount = 0; moveCount < 8; ++moveCount) {
        auto best = recommendMoves(maxDepth, 1, numThreads);
        if (best.empty()) {
            std::cout << "No move found. Game over.\n";
            break;
        }
        std::string mv = best[0].first;
        std::cout << "  Move " << moveCount + 1 << " (" << (isWhiteTurn ? "White" : "Black") << "): " << mv << std::endl;
        int code;
        movePiece(mv, code);
    }
    auto end = std::chrono::high_resolution_clock::now();
    double elapsed = std::chrono::duration<double>(end - start).count();
    std::cout << "Threads: " << (numThreads == 0 ? 1 : numThreads)
              << ", Depth: " << maxDepth
              << ", Time: " << elapsed << " sec\n";
    sharedBoardString = original;
    isWhiteTurn = originalTurn;
    rebuildBoard();
}