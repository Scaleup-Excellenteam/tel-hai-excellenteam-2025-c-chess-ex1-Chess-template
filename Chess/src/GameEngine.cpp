#include "GameEngine.h"
#include "Chess.h"
#include "Utils/Colors.h"

#include <iostream>
#include <string>
#include <vector>
#include <chrono>
#include <fstream>
#include <ctime>
#include <iomanip>
#include <limits>
#include "Pieces/King.h"
#include "Pieces/Rook.h"


// --- Constructor ---
GameEngine::GameEngine()
  : m_searchDepth(3), isWhiteTurn_(true), m_codeResponse(-1)
{
    initGame();
}

// --- Helper for putting pieces ---
void GameEngine::putPiece(int row, int col, Piece* raw) {
    board->setPiece(row, col, std::unique_ptr<Piece>(raw));
}

// --- initGame - Initializes the board with starting pieces ---
void GameEngine::initGame()
{
    board = std::make_unique<Board>();

    putPiece(0, 0, new Rook  (false));
    putPiece(0, 1, new Knight(false));
    putPiece(0, 2, new Bishop(false));
    putPiece(0, 3, new Queen (false));
    putPiece(0, 4, new King  (false));
    putPiece(0, 5, new Bishop(false));
    putPiece(0, 6, new Knight(false));
    putPiece(0, 7, new Rook  (false));

    for (int c = 0; c < 8; ++c)
        putPiece(1, c, new Pawn(false));

    for (int c = 0; c < 8; ++c)
        putPiece(6, c, new Pawn(true));

    putPiece(7, 0, new Rook  (true));
    putPiece(7, 1, new Knight(true));
    putPiece(7, 2, new Bishop(true));
    putPiece(7, 3, new Queen (true));
    putPiece(7, 4, new King  (true));
    putPiece(7, 5, new Bishop(true));
    putPiece(7, 6, new Knight(true));
    putPiece(7, 7, new Rook  (true));
}

// --- Set internal code response for UI feedback ---
void GameEngine::setCodeResponse(int code)
{
    m_codeResponse = code;
}

// --- Get internal code response ---
int GameEngine::getCodeResponse() const
{
    return m_codeResponse;
}

// --- isCheck - returns true if the current player's king is in check ---
bool GameEngine::isCheck() const
{
    return board->inCheck(isWhiteTurn_);
}

// --- makeMove (string version) - validates & applies a move, sets codeResponse, swaps turns on success ---
void GameEngine::makeMove(const std::string& moveStr)
{
    int code = validateMove(moveStr);
    setCodeResponse(code);

    if (code == 42 || code == 41) {
        int srcRow = moveStr[0] - 'a';
        int srcCol = moveStr[1] - '1';
        int destRow = moveStr[2] - 'a';
        int destCol = moveStr[3] - '1';

        // Get the piece *before* it's moved from src, to update its hasMoved_ flag
        // and to check if it's a King for castling handling in Board::applyMove
        // The piece itself (pointed to by Piece*) will be correctly updated.
        Piece* piece_at_src = board->getPiece(srcRow, srcCol);

        // If it's a King or Rook, directly set its hasMoved_ property
        if (King* k = dynamic_cast<King*>(piece_at_src)) {
            // k->setHasMoved(true);
            
        } else if (Rook* r = dynamic_cast<Rook*>(piece_at_src)) {
            r->setHasMoved(true);
        }
        
        // Apply the move (and potentially the rook's move for castling if it's a king move)
        // Board::applyMove will internally handle removing from src and placing at dest.
        board->applyMove({srcRow, srcCol, destRow, destCol});

        isWhiteTurn_ = !isWhiteTurn_; // Switch turn after a successful move application
    }
}

// --- makeMove (coordinate version) - applies a move, mutates board, switches turns ---
// Note: This overload is called internally by makeMove(string).
// The hasMoved_ logic has been consolidated in makeMove(string) before applyMove.
// This function's role is just to tell the board to apply the move.
bool GameEngine::makeMove(int srcRow, int srcCol, int destRow, int destCol)
{
    if (!board) return false;

    // The hasMoved_ update for King/Rook is now done in the `makeMove(string)` overload
    // before `board->applyMove` is called. This avoids redundant dynamic_casting here.
    board->applyMove({srcRow, srcCol, destRow, destCol});

    isWhiteTurn_ = !isWhiteTurn_; // Switch turn after a successful move application

    return true;
}


// --- validateMove - checks move legality and potential side effects ---
int GameEngine::validateMove(const std::string& mv) const
{
    if (mv.size() != 4) return 11;

    int sR = mv[0] - 'a', sC = mv[1] - '1';
    int dR = mv[2] - 'a', dC = mv[3] - '1';
    if (sR < 0 || sR >= 8 || sC < 0 || sC >= 8 || dR < 0 || dR >= 8 || dC < 0 || dC >= 8) return 11;

    Piece* src = board->getPiece(sR, sC);
    if (!src) return 11;
    if (src->getIsWhite() != isWhiteTurn_) return 12;

    if (Piece* dst = board->getPiece(dR, dC);
        dst && dst->getIsWhite() == src->getIsWhite()) return 13;

    // Check for castling as a special case *before* generic isValidMove
    if (King* king_piece = dynamic_cast<King*>(src)) {
        if (std::abs(dR - sR) == 0 && std::abs(dC - sC) == 2) { // Horizontal 2-square move (potential castling)
            if (!king_piece->isValidMove(sR, sC, dR, dC, *board)) {
                return 21; // Illegal movement for that piece (failed castling conditions)
            }
        } else if (!king_piece->isValidMove(sR, sC, dR, dC, *board)) { // Not castling, but normal king move invalid
            return 21;
        }
    } else if (!src->isValidMove(sR, sC, dR, dC, *board)) { // Other pieces' invalid moves
        return 21;
    }

    // Create a temporary board to check for self-check
    Board temp_board = *board;
    temp_board.applyMove({sR, sC, dR, dC});
    if (temp_board.inCheck(isWhiteTurn_)) {
        return 31; // this movement will cause you check
    }

    if (temp_board.inCheck(!isWhiteTurn_)) { // Check if opponent's king is in check after the move
        return 41; // the legal movement causes check
    }

    return 42; // Legal move
}

// --- isCheckmate - checks if the current player is in checkmate ---
bool GameEngine::isCheckmate() const
{
    if (!board->inCheck(isWhiteTurn_)) return false;
    auto legal = board->generateLegalMoves(isWhiteTurn_);
    return legal.empty();
}

// --- isStalemate - checks if the current player is in stalemate ---
bool GameEngine::isStalemate() const
{
    if (board->inCheck(isWhiteTurn_)) return false;
    auto legal = board->generateLegalMoves(isWhiteTurn_);
    return legal.empty();
}


bool GameEngine::isInsufficientMaterial() const {
    int whiteMinorPieces = 0; // Count of white Knights and Bishops
    int blackMinorPieces = 0; // Count of black Knights and Bishops
    bool whiteHasBishop = false; // To differentiate K+B from K+N
    bool blackHasBishop = false; // To differentiate K+B from K+N

    for (int r = 0; r < 8; ++r) {
        for (int c = 0; c < 8; ++c) {
            const Piece* p = board->getPiece(r, c);
            if (p) {
                // If any side has a pawn, rook, or queen, it's NOT insufficient material.
                // We can return false immediately without further checks.
                if (p->getSymbol() == Colors::Pieces::WHITE_PAWN || p->getSymbol() == Colors::Pieces::BLACK_PAWN ||
                    p->getSymbol() == Colors::Pieces::WHITE_ROOK || p->getSymbol() == Colors::Pieces::BLACK_ROOK ||
                    p->getSymbol() == Colors::Pieces::WHITE_QUEEN || p->getSymbol() == Colors::Pieces::BLACK_QUEEN) {
                    return false;
                }

                // Count minor pieces (Knights and Bishops)
                if (p->getIsWhite()) {
                    if (p->getSymbol() == Colors::Pieces::WHITE_KNIGHT) whiteMinorPieces++;
                    else if (p->getSymbol() == Colors::Pieces::WHITE_BISHOP) {
                        whiteMinorPieces++;
                        whiteHasBishop = true;
                    }
                } else { // Black piece
                    if (p->getSymbol() == Colors::Pieces::BLACK_KNIGHT) blackMinorPieces++;
                    else if (p->getSymbol() == Colors::Pieces::BLACK_BISHOP) {
                        blackMinorPieces++;
                        blackHasBishop = true;
                    }
                }
            }
        }
    }

    // After the loop, we know there are no pawns, rooks, or queens.
    // Only Kings, Knights, and Bishops remain on the board.

    // Case 1: King vs King (no other pieces on board)
    if (whiteMinorPieces == 0 && blackMinorPieces == 0) {
        return true;
    }

    // Case 2: King and one minor piece vs King
    // This covers K+N vs K, K+B vs K for either white or black.
    if ((whiteMinorPieces == 1 && blackMinorPieces == 0) ||
        (blackMinorPieces == 1 && whiteMinorPieces == 0)) {
        return true;
    }

    // Case 3: King and one minor piece vs King and one minor piece
    // This covers K+N vs K+N, K+B vs K+B, K+N vs K+B.
    if (whiteMinorPieces == 1 && blackMinorPieces == 1) {
        return true;
    }

    return false; // Any other combination (e.g., K+2N vs K) is considered sufficient.
}

// --- displayWelcomeBanner ---
void GameEngine::displayWelcomeBanner() const {
    using namespace Colors;
    std::cout << INFO_COLOR << "╔═══════════════════════════════════════╗" << RESET << std::endl;
    std::cout << INFO_COLOR << "║" << RESET;
    std::cout << YELLOW << "          ♔ CHESS GAME ♚             " << RESET;
    std::cout << INFO_COLOR << "║" << RESET << std::endl;
    std::cout << INFO_COLOR << "║" << RESET;
    std::cout << INFO_COLOR << "      Classic Strategy Game v1.0      " << RESET;
    std::cout << INFO_COLOR << "║" << RESET << std::endl;
    std::cout << INFO_COLOR << "╚═══════════════════════════════════════╝" << RESET << std::endl << std::endl;
}

// --- getUserSettings ---
void GameEngine::getUserSettings() {
    using namespace Colors;
    int gameMode;

    std::cout << MENU_COLOR << "◆ Enter AI search depth " << GREEN << "(1-5, recommended 3): " << RESET;
    std::cin >> m_searchDepth;
    if (std::cin.fail() || std::cin.peek() != '\n' || m_searchDepth < 1 || m_searchDepth > 5) { // Added peek() check
        std::cout << RED << "✗ Invalid input. Defaulting to depth 3." << RESET << std::endl;
        std::cin.clear();
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        m_searchDepth = 3;
    } else {
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n'); // Consume newline if input was good
    }


    std::cout << std::endl << MENU_COLOR << "◆ Select game mode:" << RESET << std::endl;
    std::cout << CYAN << "  1. " << RESET << "Interactive Game (Player vs. Player)" << std::endl;
    std::cout << CYAN << "  2. " << RESET << "Automatic Benchmark" << std::endl;
    std::cout << MENU_COLOR << "◆ Enter mode (1 or 2): " << RESET;
    std::cin >> gameMode;
    if (std::cin.fail() || std::cin.peek() != '\n' || (gameMode != 1 && gameMode != 2)) { // Added peek() check
        std::cout << RED << "✗ Invalid mode. Defaulting to Interactive Game." << RESET << std::endl;
        std::cin.clear();
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        gameMode = 1;
    } else {
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n'); // Consume newline if input was good
    }


    if (gameMode == 2) {
        std::cout << INFO_COLOR << "\n◆ Starting benchmark mode..." << RESET << std::endl;
        runBenchmark(m_searchDepth);
    } else {
        runInteractiveGame();
    }
}

// --- runInteractiveGame - Main game loop for interactive play ---
// --- runInteractiveGame - Main game loop for interactive play ---
void GameEngine::runInteractiveGame() {
    using namespace Colors;
    std::cout << CYAN << "\n✓ Starting interactive game..." << RESET << std::endl;
    std::cout << BOARD_FRAME << "━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━" << RESET << std::endl;
    std::cout << CYAN << "◆ Enter moves in format: " << ORANGE << "a2a4" << RESET << std::endl;
    std::cout << CYAN << "◆ Type " << ORANGE << "'exit'" << CYAN << " or " << ORANGE << "'quit'" << CYAN << " to end game" << RESET << std::endl;
    std::cout << BOARD_FRAME << "━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━" << RESET << std::endl << std::endl;

    std::string initial_board_str = "RNBQKBNRPPPPPPPP################################pppppppprnbqkbnr";
    Chess game_ui(initial_board_str, this);

    std::string userInput;

    while (true) {
        if (isCheckmate()) {
            game_ui.setCodeResponse(99);
            game_ui.displayBoard();
            std::cout << RED << BOLD << "CHECKMATE! " << (isWhiteTurn_ ? "Black" : "White") << " wins!" << RESET << std::endl;
            break;
        } else if (isStalemate()) {
            game_ui.setCodeResponse(98);
            game_ui.displayBoard();
            std::cout << BLUE << BOLD << "STALEMATE! Game is a draw." << RESET << std::endl;
            break;
        }
        // ADDED Insufficient Material check
        else if (isInsufficientMaterial()) {
            game_ui.setCodeResponse(98); // Use 98 for draw
            game_ui.displayBoard();
            std::cout << BLUE << BOLD << "DRAW! Insufficient material." << RESET << std::endl;
            break;
        }

        userInput = game_ui.getInput();

        if (userInput == "exit") {
            break;
        }

        makeMove(userInput);

        game_ui.setCodeResponse(m_codeResponse);
    }

    std::cout << std::endl << BOARD_FRAME << "━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━" << RESET << std::endl;
    std::cout << CYAN << "◆ Thanks for playing! Goodbye!" << RESET << std::endl;
    std::cout << BOARD_FRAME << "━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━" << RESET << std::endl;
}

// --- runBenchmark - Simulates a short game and appends results to README.md ---
void GameEngine::runBenchmark(int searchDepth) {
    std::ofstream readme_file("../README.md", std::ios::app);
    if (!readme_file.is_open()) {
        std::cerr << "Warning: Could not open ../README.md. Trying current directory." << std::endl;
        readme_file.open("README.md", std::ios::app);
    }

    auto now = std::chrono::system_clock::now();
    auto in_time_t = std::chrono::system_clock::to_time_t(now);
    std::tm buf{};
#ifdef _WIN32
    localtime_s(&buf, &in_time_t);
#else
    localtime_r(&in_time_t, &buf);
#endif

    std::stringstream header_stream;
    header_stream << "\n\n## Benchmark Results (" << std::put_time(&buf, "%Y-%m-%d %H:%M:%S") << ")\n\n";
    header_stream << "AI Search Depth: **" << searchDepth << "**\n\n";
    header_stream << "| Threads | Total Time (ms) | Average Time per Move (ms) |\n";
    header_stream << "|:-------:|:---------------:|:--------------------------:|\n";

    std::cout << "\n--- Starting Benchmark ---\n";
    std::cout << header_stream.str();
    if (readme_file.is_open()) {
        readme_file << header_stream.str();
    }

    std::vector<std::string> gameMoves = {
        "g2g4", "b7b5", "h2f3", "b8c6", "h5d7", "a7a6", "d7c6", "b7xc6"
    };
    std::vector<int> threadCounts = {1, 2, 4, 8};

    for (int threads : threadCounts) {
        GameEngine temp_engine;
        long long total_duration_ms = 0;

        for (const auto& moveStr : gameMoves) {
            temp_engine.makeMove(moveStr);
            auto start_time = std::chrono::high_resolution_clock::now();
            AI::findBestMoves(temp_engine.currentBoard(), temp_engine.whiteToMove(), searchDepth, threads);
            auto end_time = std::chrono::high_resolution_clock::now();
            total_duration_ms += std::chrono::duration_cast<std::chrono::milliseconds>(end_time - start_time).count();
        }

        long long average_duration = total_duration_ms / gameMoves.size();

        std::stringstream result_stream;
        result_stream << "| " << std::setw(7) << threads << " | "
                      << std::setw(15) << total_duration_ms << " | "
                      << std::setw(26) << average_duration << " |\n";

        std::cout << result_stream.str();
        if (readme_file.is_open()) {
            readme_file << result_stream.str();
        }
    }

    std::cout << "\nBenchmark finished. Results appended to README.md.\n";
    if (readme_file.is_open()) {
        readme_file << "\n---\n";
        readme_file.close();
    }
}

// --- run - Main application entry point ---
void GameEngine::run() {
    displayWelcomeBanner();
    getUserSettings();
}