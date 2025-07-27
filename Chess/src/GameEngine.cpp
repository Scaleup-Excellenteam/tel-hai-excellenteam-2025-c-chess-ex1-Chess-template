// Chess/src/GameEngine.cpp
#include "GameEngine.h"
#include "Chess.h"
#include "Utils/Colors.h"
#include "AI/BestMoveFinder.h"
#include "Utils/CMove.h"


#include <iostream>
#include <string>
#include <vector>
#include <chrono>
#include <fstream>
#include <ctime>
#include <iomanip>
#include <limits>
#include <thread>
#include "Pieces/King.h"
#include "Pieces/Rook.h"
#include "Pieces/Queen.h"
#include "Pieces/Bishop.h"
#include "Pieces/Knight.h"
#include "Pieces/Pawn.h"

// --- Constructor ---
GameEngine::GameEngine()
  : m_searchDepth(3), isWhiteTurn_(true), m_codeResponse(-1), m_playAgainstAI(false),
    m_halfMoveClock(0)
{
    initGame();
}

// --- Helper for putting pieces (kept private, test will use setPieceForTest) ---
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

    // Clear board history and reset half-move clock for a new game
    m_boardHistory.clear();
    m_halfMoveClock = 0;
    // Add initial state to history for repetition tracking
    m_boardHistory[getBoardStateString()] = 1;
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

// --- handlePawnPromotion ---
void GameEngine::handlePawnPromotion(int row, int col) {
    Piece* pawnToPromote = board->getPiece(row, col);
    if (!pawnToPromote || dynamic_cast<Pawn*>(pawnToPromote) == nullptr) {
        return;
    }

    bool isWhitePawn = pawnToPromote->getIsWhite();
    char choiceChar;

    if (m_playAgainstAI && !isWhiteTurn_) { // It was AI's turn (Black AI)
        board->setPiece(row, col, std::make_unique<Queen>(isWhitePawn));
        std::cout << Colors::INFO_COLOR << "AI promoted pawn to Queen!" << Colors::RESET << std::endl;
    } else { // Human player's turn (White human or Black human in PvP)
        std::cout << Colors::INFO_COLOR << "\nPawn promotion! Choose piece (Q=Queen, R=Rook, B=Bishop, N=Knight): " << Colors::RESET;
        std::cin >> choiceChar;
        choiceChar = std::tolower(static_cast<unsigned char>(choiceChar));

        std::unique_ptr<Piece> promotedPiece;
        switch (choiceChar) {
            case 'r': promotedPiece = std::make_unique<Rook>(isWhitePawn); break;
            case 'b': promotedPiece = std::make_unique<Bishop>(isWhitePawn); break;
            case 'n': promotedPiece = std::make_unique<Knight>(isWhitePawn); break;
            case 'q':
            default:
                promotedPiece = std::make_unique<Queen>(isWhitePawn);
                if (choiceChar != 'q') {
                    std::cout << Colors::YELLOW << "Invalid choice. Defaulting to Queen." << Colors::RESET << std::endl;
                }
                break;
        }
        board->setPiece(row, col, std::move(promotedPiece));
    }
}


// --- makeMove (string version) - validates & applies a move, sets codeResponse, swaps turns on success ---
void GameEngine::makeMove(const std::string& moveStr)
{
    int srcRow = moveStr[0] - 'a';
    int srcCol = moveStr[1] - '1';
    int destRow = moveStr[2] - 'a';
    int destCol = moveStr[3] - '1';

    Piece* piece_at_src = board->getPiece(srcRow, srcCol);
    Piece* piece_at_dest_before_move = board->getPiece(destRow, destCol);

    bool isPawnMove = (dynamic_cast<Pawn*>(piece_at_src) != nullptr);
    bool isCapture = (piece_at_dest_before_move != nullptr);
    bool isEnPassantCapture = false;
    if (isPawnMove && std::abs(srcCol - destCol) == 1 && piece_at_dest_before_move == nullptr) {
        if (Pawn* pawn = dynamic_cast<Pawn*>(piece_at_src)) {
            if (pawn->isValidEnPassant(srcRow, srcCol, destRow, destCol, *board)) {
                isEnPassantCapture = true;
            }
        }
    }


    int code = validateMove(moveStr);
    setCodeResponse(code);

    if (code == 42 || code == 41) {
        if (King* k = dynamic_cast<King*>(piece_at_src)) {
            k->setHasMoved(true);
        } else if (Rook* r = dynamic_cast<Rook*>(piece_at_src)) {
            r->setHasMoved(true);
        }

        board->applyMove({srcRow, srcCol, destRow, destCol});

        if (isPawnMove || isCapture || isEnPassantCapture) {
            m_halfMoveClock = 0;
        } else {
            m_halfMoveClock++;
        }

        if (isPawnMove && board->isPawnPromotionReady(destRow, destCol)) {
            handlePawnPromotion(destRow, destCol);
            m_halfMoveClock = 0;
        }

        isWhiteTurn_ = !isWhiteTurn_;

        m_boardHistory[getBoardStateString()]++;
    }
}

// --- makeMove (coordinate version) - applies a move, mutates board, switches turns ---
bool GameEngine::makeMove(int srcRow, int srcCol, int destRow, int destCol)
{
    if (!board) return false;

    board->applyMove({srcRow, srcCol, destRow, destCol});

    isWhiteTurn_ = !isWhiteTurn_;

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

    if (King* king_piece = dynamic_cast<King*>(src)) {
        if (std::abs(dR - sR) == 0 && std::abs(dC - sC) == 2) {
            if (!king_piece->isValidMove(sR, sC, dR, dC, *board)) {
                return 21;
            }
        } else if (!king_piece->isValidMove(sR, sC, dR, dC, *board)) {
            return 21;
        }
    } else if (!src->isValidMove(sR, sC, dR, dC, *board)) {
        return 21;
    }

    Board temp_board = *board;
    temp_board.applyMove({sR, sC, dR, dC});
    if (temp_board.inCheck(isWhiteTurn_)) {
        return 31;
    }

    if (temp_board.inCheck(!isWhiteTurn_)) {
        return 41;
    }

    return 42;
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

// Implements simplified insufficient material checks based on common draw scenarios.
bool GameEngine::isInsufficientMaterial() const {
    int whiteMinorPieces = 0;
    int blackMinorPieces = 0;

    for (int r = 0; r < 8; ++r) {
        for (int c = 0; c < 8; ++c) {
            const Piece* p = board->getPiece(r, c);
            if (p) {
                if (p->getSymbol() == Colors::Pieces::WHITE_PAWN || p->getSymbol() == Colors::Pieces::BLACK_PAWN ||
                    p->getSymbol() == Colors::Pieces::WHITE_ROOK || p->getSymbol() == Colors::Pieces::BLACK_ROOK ||
                    p->getSymbol() == Colors::Pieces::WHITE_QUEEN || p->getSymbol() == Colors::Pieces::BLACK_QUEEN) {
                    return false;
                }

                if (p->getIsWhite()) {
                    if (p->getSymbol() == Colors::Pieces::WHITE_KNIGHT) whiteMinorPieces++;
                    else if (p->getSymbol() == Colors::Pieces::WHITE_BISHOP) {
                        whiteMinorPieces++;
                    }
                } else {
                    if (p->getSymbol() == Colors::Pieces::BLACK_KNIGHT) blackMinorPieces++;
                    else if (p->getSymbol() == Colors::Pieces::BLACK_BISHOP) {
                        blackMinorPieces++;
                    }
                }
            }
        }
    }

    if (whiteMinorPieces == 0 && blackMinorPieces == 0) {
        return true;
    }

    if ((whiteMinorPieces == 1 && blackMinorPieces == 0) ||
        (blackMinorPieces == 1 && whiteMinorPieces == 0)) {
        return true;
    }

    if (whiteMinorPieces == 1 && blackMinorPieces == 1) {
        return true;
    }

    return false;
}

// --- isFiftyMoveDraw function ---
bool GameEngine::isFiftyMoveDraw() const {
    return m_halfMoveClock >= 100;
}

// --- getBoardStateString function for Threefold Repetition ---
std::string GameEngine::getBoardStateString() const {
    std::string state = "";
    for (int r = 0; r < 8; ++r) {
        for (int c = 0; c < 8; ++c) {
            const Piece* p = board->getPiece(r, c);
            if (p) {
                state += p->getSymbol();
            } else {
                state += '#';
            }
        }
    }

    state += (isWhiteTurn_ ? 'W' : 'B');

    const Piece* whiteKing = board->getPiece(7, 4);
    const Piece* blackKing = board->getPiece(0, 4);
    const Piece* whiteRookH = board->getPiece(7, 7);
    const Piece* whiteRookA = board->getPiece(7, 0);
    const Piece* blackRookH = board->getPiece(0, 7);
    const Piece* blackRookA = board->getPiece(0, 0);

    if (const King* wk = dynamic_cast<const King*>(whiteKing)) {
        if (!wk->getHasMoved()) {
            if (const Rook* wrh = dynamic_cast<const Rook*>(whiteRookH)) {
                if (!wrh->getHasMoved()) state += 'K';
            }
            if (const Rook* wra = dynamic_cast<const Rook*>(whiteRookA)) {
                if (!wra->getHasMoved()) state += 'Q';
            }
        }
    }
    if (const King* bk = dynamic_cast<const King*>(blackKing)) {
        if (!bk->getHasMoved()) {
            if (const Rook* brh = dynamic_cast<const Rook*>(blackRookH)) {
                if (!brh->getHasMoved()) state += 'k';
            }
            if (const Rook* bra = dynamic_cast<const Rook*>(blackRookA)) {
                if (!bra->getHasMoved()) state += 'q';
            }
        }
    }

    CMove lm = board->getLastMove();
    if (dynamic_cast<Pawn*>(board->getPiece(lm.destRow, lm.destCol)) && std::abs(lm.srcRow - lm.destRow) == 2) {
        int epRow = (lm.srcRow + lm.destRow) / 2;
        int epCol = lm.destCol;
        state += static_cast<char>('a' + epRow);
        state += static_cast<char>('1' + epCol);
    } else {
        state += '-';
    }

    return state;
}

// --- isThreefoldRepetition function ---
bool GameEngine::isThreefoldRepetition() const {
    auto it = m_boardHistory.find(getBoardStateString());
    if (it != m_boardHistory.end()) {
        return it->second >= 3;
    }
    return false;
}

// --- displayWelcomeBanner ---
void GameEngine::displayWelcomeBanner() const {
    using namespace Colors;
    std::cout << INFO_COLOR << "╔═══════════════════════════════════════╗" << RESET << std::endl;
    std::cout << INFO_COLOR << "║" << RESET;
    std::cout << YELLOW << "          ♔   CHESS GAME ♚             " << RESET;
    std::cout << INFO_COLOR << "║" << RESET << std::endl;
    std::cout << INFO_COLOR << "║" << RESET;
    std::cout << INFO_COLOR << "      Classic Strategy Game v1.0       " << RESET;
    std::cout << INFO_COLOR << "║" << RESET << std::endl;
    std::cout << INFO_COLOR << "╚═══════════════════════════════════════╝" << RESET << std::endl << std::endl;
}

// --- getUserSettings ---
void GameEngine::getUserSettings() {
    using namespace Colors;
    int gameMode;

    std::cout << MENU_COLOR << "◆ Enter AI search depth " << GREEN << "(1-5, recommended 3): " << RESET;
    std::cin >> m_searchDepth;
    if (std::cin.fail() || std::cin.peek() != '\n' || m_searchDepth < 1 || m_searchDepth > 5) {
        std::cout << RED << "✗ Invalid input. Defaulting to depth 3." << RESET << std::endl;
        std::cin.clear();
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        m_searchDepth = 3;
    } else {
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    }


    std::cout << std::endl << MENU_COLOR << "◆ Select game mode:" << RESET << std::endl;
    std::cout << CYAN << "  1. " << RESET << "Interactive Game (Player vs. Player)" << std::endl;
    std::cout << CYAN << "  2. " << RESET << "Player vs. Computer (AI)" << std::endl;
    std::cout << CYAN << "  3. " << RESET << "Automatic Benchmark" << std::endl;
    std::cout << MENU_COLOR << "◆ Enter mode (1, 2 or 3): " << RESET;
    std::cin >> gameMode;
    if (std::cin.fail() || std::cin.peek() != '\n' || (gameMode != 1 && gameMode != 2 && gameMode != 3)) {
        std::cout << RED << "✗ Invalid mode. Defaulting to Interactive Game." << RESET << std::endl;
        std::cin.clear();
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        gameMode = 1;
    } else {
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    }

    if (gameMode == 3) {
        runBenchmark(m_searchDepth);
    } else if (gameMode == 2) {
        m_playAgainstAI = true;
        std::cout << Colors::INFO_COLOR << "\n◆ Starting Player vs. Computer game..." << Colors::RESET << std::endl;
        runInteractiveGame();
    }
    else { // Player vs Player
        m_playAgainstAI = false;
        runInteractiveGame();
    }
}

// --- runInteractiveGame - Main game loop for interactive play ---
void GameEngine::runInteractiveGame() {
    using namespace Colors;
    std::cout << CYAN << "\n✓ Starting interactive game..." << RESET << std::endl;
    std::cout << BOARD_FRAME << "━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━" << RESET << std::endl;
    std::cout << CYAN << "◆ Enter moves in format: " << ORANGE << "a2a4" << RESET << std::endl;
    std::cout << CYAN << "◆ Type " << ORANGE << "'exit'" << CYAN << " or " << ORANGE << "'quit'" << CYAN << " to end game" << std::endl;
    std::cout << BOARD_FRAME << "━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━" << RESET << std::endl << std::endl;

    std::string initial_board_str = "RNBQKBNRPPPPPPPP################################pppppppprnbqkbnr";
    Chess game_ui(initial_board_str, this);

    // Initial board state for repetition tracking (already done in initGame)
    // m_boardHistory[getBoardStateString()] = 1;

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
        else if (isInsufficientMaterial()) {
            game_ui.setCodeResponse(98);
            game_ui.displayBoard();
            std::cout << BLUE << BOLD << "DRAW! Insufficient material." << RESET << std::endl;
            break;
        }
        else if (isFiftyMoveDraw()) {
            game_ui.setCodeResponse(98);
            game_ui.displayBoard();
            std::cout << BLUE << BOLD << "DRAW! 50-move rule." << RESET << std::endl;
            break;
        }
        else if (isThreefoldRepetition()) {
            game_ui.setCodeResponse(98);
            game_ui.displayBoard();
            std::cout << BLUE << BOLD << "DRAW! Threefold repetition." << RESET << std::endl;
            break;
        }


        bool isAITurn = (m_playAgainstAI && !isWhiteTurn_);

        if (isAITurn) {
            game_ui.displayBoard();
            std::cout << Colors::INFO_COLOR << "AI (Black) is thinking..." << Colors::RESET << std::endl;

            auto bestMoves = AI::findBestMoves(currentBoard(), false, 1, 8, m_searchDepth);

            if (!bestMoves.empty()) {
                CMove aiMove = bestMoves[0].move;
                userInput = "";
                userInput += (char)('a' + aiMove.srcRow);
                userInput += (char)('1' + aiMove.srcCol);
                userInput += (char)('a' + aiMove.destRow);
                userInput += (char)('1' + aiMove.destCol);
                std::cout << Colors::INFO_COLOR << "AI moves: " << Colors::BOLD << userInput << Colors::RESET << std::endl;
                std::this_thread::sleep_for(std::chrono::milliseconds(1000));
            } else {
                std::cout << Colors::RED << "AI has no legal moves. Game ends." << Colors::RESET << std::endl;
                break;
            }
        } else {
            userInput = game_ui.getInput();
            if (userInput == "exit") {
                break;
            }
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
            AI::findBestMoves(temp_engine.currentBoard(), temp_engine.whiteToMove(), searchDepth, threads, searchDepth);
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

// --- PUBLIC METHODS FOR TESTING ONLY ---
void GameEngine::resetGameForTest() {
    board = std::make_unique<Board>();
    isWhiteTurn_ = true;
    m_halfMoveClock = 0;
    m_boardHistory.clear();
}

void GameEngine::setPieceForTest(int row, int col, Piece* rawPiece) {
    board->setPiece(row, col, std::unique_ptr<Piece>(rawPiece));
}

void GameEngine::setTurnForTest(bool isWhite) {
    isWhiteTurn_ = isWhite;
}

void GameEngine::setHalfMoveClockForTest(int count) {
    m_halfMoveClock = count;
}

void GameEngine::clearBoardHistoryForTest() {
    m_boardHistory.clear();
}

void GameEngine::addBoardStateToHistoryForTest() {
    m_boardHistory[getBoardStateString()]++;
}

void GameEngine::setPlayAgainstAIForTest(bool value) {
    m_playAgainstAI = value;
}


void GameEngine::setLastMoveForTest(CMove move) {
    // Corrected: Call the public setter on the board object
    board->setLastMoveForTest(move);
}