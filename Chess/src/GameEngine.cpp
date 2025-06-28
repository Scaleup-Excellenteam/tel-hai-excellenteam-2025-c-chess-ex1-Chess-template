// Chess/src/GameEngine.cpp
#include "GameEngine.h"
#include "Chess.h" // Include Chess.h now as GameEngine will create/interact with Chess UI
#include "Utils/Colors.h"

#include <iostream>
#include <string>
#include <vector>
#include <chrono>
#include <fstream>   // Required for file I/O
#include <ctime>     // Required for timestamp
#include <iomanip>   // Required for formatting time
#include <limits>    // For std::numeric_limits

// --- Constructor ---
GameEngine::GameEngine()
  : m_searchDepth(3), isWhiteTurn_(true), m_codeResponse(-1)
{
    initGame(); // Initialize the board and pieces when engine is created
}

// --- Helper for putting pieces (migrated from GameManager's lambda) ---
void GameEngine::putPiece(int row, int col, Piece* raw) {
    board->setPiece(row, col, std::unique_ptr<Piece>(raw));
}

// --- initGame - Initializes the board with starting pieces ---
void GameEngine::initGame()
{
    /* 1. Create a fresh board */
    board = std::make_unique<Board>();

    /* 3. Black back rank (row 0) – UPPER-case symbols */
    putPiece(0, 0, new Rook  (false));   // A8
    putPiece(0, 1, new Knight(false));   // B8
    putPiece(0, 2, new Bishop(false));   // C8
    putPiece(0, 3, new Queen (false));   // D8
    putPiece(0, 4, new King  (false));   // E8
    putPiece(0, 5, new Bishop(false));   // F8
    putPiece(0, 6, new Knight(false));   // G8
    putPiece(0, 7, new Rook  (false));   // H8

    /* 4. Black pawns (row 1) */
    for (int c = 0; c < 8; ++c)
        putPiece(1, c, new Pawn(false));           // A7 .. H7

    /* 5. White pawns (row 6) – lower-case symbols */
    for (int c = 0; c < 8; ++c)
        putPiece(6, c, new Pawn(true));            // A2 .. H2

    /* 6. White back rank (row 7) */
    putPiece(7, 0, new Rook  (true));    // A1
    putPiece(7, 1, new Knight(true));    // B1
    putPiece(7, 2, new Bishop(true));    // C1
    putPiece(7, 3, new Queen (true));    // D1
    putPiece(7, 4, new King  (true));    // E1
    putPiece(7, 5, new Bishop(true));    // F1
    putPiece(7, 6, new Knight(true));    // G1
    putPiece(7, 7, new Rook  (true));    // H1
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
    setCodeResponse(code); // Update the internal code for the UI

    if (code == 42 || code == 41) { // If move is legal (causes check or not)
        int srcRow = moveStr[0] - 'a';
        int srcCol = moveStr[1] - '1';
        int destRow = moveStr[2] - 'a';
        int destCol = moveStr[3] - '1';
        // Apply the move using the low-level makeMove
        makeMove(srcRow, srcCol, destRow, destCol); // This internal call flips isWhiteTurn_
    }
}

// --- makeMove (coordinate version) - applies a move, mutates board, switches turns ---
bool GameEngine::makeMove(int srcRow, int srcCol, int destRow, int destCol)
{
    if (!board) return false;

    // Apply the move using Board's applyMove
    board->applyMove({srcRow, srcCol, destRow, destCol});

    isWhiteTurn_ = !isWhiteTurn_; // Switch turn after a successful move application

    return true;
}

// --- validateMove - checks move legality and potential side effects ---
int GameEngine::validateMove(const std::string& mv) const
{
    if (mv.size() != 4) return 11; // Invalid format or no piece (initial check)

    int sR = mv[0] - 'a', sC = mv[1] - '1';
    int dR = mv[2] - 'a', dC = mv[3] - '1';
    if (sR < 0 || sR >= 8 || sC < 0 || sC >= 8 || dR < 0 || dR >= 8 || dC < 0 || dC >= 8) return 11; // Out of bounds

    Piece* src = board->getPiece(sR, sC);
    if (!src) return 11;                           // No piece at source
    if (src->getIsWhite() != isWhiteTurn_) return 12; // Opponent’s piece

    if (Piece* dst = board->getPiece(dR, dC);
        dst && dst->getIsWhite() == src->getIsWhite()) return 13; // Own piece at destination

    if (!src->isValidMove(sR, sC, dR, dC, *board)) return 21; // Piece-specific illegal movement

    // Create a temporary board to check for self-check
    Board temp_board = *board;
    temp_board.applyMove({sR, sC, dR, dC}); // Apply potential move
    if (temp_board.inCheck(isWhiteTurn_)) {
        return 31; // This movement would leave your king in check
    }

    // Check if this move puts the opponent in check
    if (temp_board.inCheck(!isWhiteTurn_)) { // Check if opponent's king is in check after the move
        return 41; // The legal movement causes check
    }

    return 42; // Legal move
}

// --- isCheckmate - checks if the current player is in checkmate ---
bool GameEngine::isCheckmate() const
{
    if (!board->inCheck(isWhiteTurn_)) return false; // Not checkmate if not in check
    auto legal = board->generateLegalMoves(isWhiteTurn_); // Get all legal moves for current player
    return legal.empty(); // Checkmate if no legal moves
}

// --- isStalemate - checks if the current player is in stalemate ---
bool GameEngine::isStalemate() const
{
    if (board->inCheck(isWhiteTurn_)) return false; // Not stalemate if in check
    auto legal = board->generateLegalMoves(isWhiteTurn_); // Get all legal moves for current player
    return legal.empty(); // Stalemate if no legal moves and not in check
}

// --- whiteToMove - returns true if it's White's turn ---
// This method is defined inline in GameEngine.h, so no redefinition here.
// bool GameEngine::whiteToMove() const { return isWhiteTurn_; }

// --- displayWelcomeBanner ---
void GameEngine::displayWelcomeBanner() const {
    using namespace Colors;
    std::cout << INFO_COLOR << "╔═══════════════════════════════════════╗" << RESET << std::endl;
    std::cout << INFO_COLOR << "║" << RESET;
    std::cout << YELLOW << "            ♔ CHESS GAME ♚             " << RESET;
    std::cout << INFO_COLOR << "║" << RESET << std::endl;
    std::cout << INFO_COLOR << "║" << RESET;
    std::cout << INFO_COLOR << "       Classic Strategy Game v1.0      " << RESET;
    std::cout << INFO_COLOR << "║" << RESET << std::endl;
    std::cout << INFO_COLOR << "╚═══════════════════════════════════════╝" << RESET << std::endl << std::endl;
}

// --- getUserSettings ---
void GameEngine::getUserSettings() {
    using namespace Colors;
    int gameMode;

    std::cout << MENU_COLOR << "◆ Enter AI search depth " << GREEN << "(1-5, recommended 3): " << RESET;
    std::cin >> m_searchDepth;
    if (std::cin.fail() || m_searchDepth < 1 || m_searchDepth > 5) {
        std::cout << RED << "✗ Invalid input. Defaulting to depth 3." << RESET << std::endl;
        std::cin.clear(); // Clear error flags
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n'); // Discard invalid input
        m_searchDepth = 3;
    }

    std::cout << std::endl << MENU_COLOR << "◆ Select game mode:" << RESET << std::endl;
    std::cout << CYAN << "  1. " << RESET << "Interactive Game (Player vs. Player)" << std::endl;
    std::cout << CYAN << "  2. " << RESET << "Automatic Benchmark" << std::endl;
    std::cout << MENU_COLOR << "◆ Enter mode (1 or 2): " << RESET;
    std::cin >> gameMode;
    if (std::cin.fail() || (gameMode != 1 && gameMode != 2)) {
        std::cout << RED << "✗ Invalid mode. Defaulting to Interactive Game." << RESET << std::endl;
        std::cin.clear();
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        gameMode = 1;
    }

    if (gameMode == 2) {
        std::cout << INFO_COLOR << "\n◆ Starting benchmark mode..." << RESET << std::endl;
        runBenchmark(m_searchDepth);
    } else {
        runInteractiveGame();
    }
}

// --- runInteractiveGame - Main game loop for interactive play ---
void GameEngine::runInteractiveGame() {
    using namespace Colors;
    std::cout << CYAN << "\n✓ Starting interactive game..." << RESET << std::endl;
    std::cout << BOARD_FRAME << "━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━" << RESET << std::endl;
    std::cout << CYAN << "◆ Enter moves in format: " << ORANGE << "a2a4" << RESET << std::endl;
    std::cout << CYAN << "◆ Type " << ORANGE << "'exit'" << CYAN << " or " << ORANGE << "'quit'" << CYAN << " to end game" << RESET << std::endl;
    std::cout << BOARD_FRAME << "━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━" << RESET << std::endl << std::endl;

    std::string initial_board_str = "RNBQKBNRPPPPPPPP################################pppppppprnbqkbnr";
    // Create the Chess UI object, passing a pointer to this GameEngine instance
    Chess game_ui(initial_board_str, this);

    std::string userInput;

    // Game loop
    while (true) {
        // First, check for game end conditions before prompting for input
        if (isCheckmate()) {
            game_ui.setCodeResponse(99); // Special code for checkmate
            game_ui.displayBoard();
            std::cout << RED << BOLD << "CHECKMATE! " << (isWhiteTurn_ ? "Black" : "White") << " wins!" << RESET << std::endl;
            break; // Exit game loop
        } else if (isStalemate()) {
            game_ui.setCodeResponse(98); // Special code for stalemate
            game_ui.displayBoard();
            std::cout << BLUE << BOLD << "STALEMATE! Game is a draw." << RESET << std::endl;
            break; // Exit game loop
        }

        // Get input from UI, which also handles displaying the board and prompts
        userInput = game_ui.getInput();

        if (userInput == "exit") {
            break; // User wants to exit
        }

        // Validate and make the move
        makeMove(userInput); // This calls validateMove and sets m_codeResponse internally
                             // and applies the move if valid, flipping the turn.

        // Pass the result code back to the UI for display
        game_ui.setCodeResponse(m_codeResponse);
    }

    std::cout << std::endl << BOARD_FRAME << "━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━" << RESET << std::endl;
    std::cout << CYAN << "◆ Thanks for playing! Goodbye! ◆ " << RESET << std::endl;
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
    localtime_r(&in_time_t, &buf); // POSIX-compliant
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

    // A consistent, legal set of moves for benchmarking
    std::vector<std::string> gameMoves = {
        "g2g4", "b7b5", "h2f3", "b8c6", "h5d7", "a7a6", "d7c6", "b7xc6"
    };
    std::vector<int> threadCounts = {1, 2, 4, 8};

    for (int threads : threadCounts) {
        // Create a new GameEngine instance for each thread count to reset board state
        GameEngine temp_engine; // Fresh board for each benchmark run
        long long total_duration_ms = 0;

        for (const auto& moveStr : gameMoves) {
            temp_engine.makeMove(moveStr); // Apply the move
            auto start_time = std::chrono::high_resolution_clock::now();
            // Call AI to find best moves for the current state
            AI::findBestMoves(temp_engine.currentBoard(), temp_engine.whiteToMove(), searchDepth, threads); // Use the provided searchDepth
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
    getUserSettings(); // This will call either runInteractiveGame or runBenchmark
}