// Chess/src/Chess.cpp
#include "Chess.h"
#include "GameEngine.h"
#include "AI/BestMoveFinder.h"
#include "Utils/Colors.h"
#include "Utils/CMove.h" // Needed for CMove in AI calls
#include <iostream>
#include <string>
#include <cctype>
#include <iomanip>
#include <chrono> // For std::chrono::milliseconds
#include <thread> // For std::this_thread::sleep_for


using namespace std;
using namespace Colors;

#ifdef _WIN32

void Chess::clear() const
{
	COORD topLeft = { 0, 0 };
	HANDLE console = GetStdHandle(STD_OUTPUT_HANDLE);
	CONSOLE_SCREEN_BUFFER_INFO screen;
	DWORD written;

	GetConsoleScreenBufferInfo(console, &screen);
	FillConsoleOutputCharacterA(
		console, ' ', screen.dwSize.X * screen.dwSize.Y, topLeft, &written
	);
	FillConsoleOutputAttribute(
		console, FOREGROUND_GREEN | FOREGROUND_RED | FOREGROUND_BLUE,
		screen.dwSize.X * screen.dwSize.Y, topLeft, &written
	);
	SetConsoleCursorPosition(console, topLeft);
}

void Chess::setFrames()
{
    // Initialize entire m_board with spaces
    for (size_t row = 0; row < BOARD_DISPLAY_GRID_SIZE; ++row) {
        for (size_t col = 0; col < BOARD_DISPLAY_GRID_SIZE; ++col) {
            m_board[row][col] = " ";
        }
    }

    // Outer Frame Borders
    for (size_t col = 0; col < BOARD_DISPLAY_GRID_COLS; ++col) {
        m_board[0][col] = (col == 0 || col == BOARD_DISPLAY_GRID_COLS - 1) ? "+" : "-"; // Top border
        m_board[BOARD_DISPLAY_GRID_ROWS - 1][col] = (col == 0 || col == BOARD_DISPLAY_GRID_COLS - 1) ? "+" : "-"; // Bottom border
    }
    for (size_t row = 1; row < BOARD_DISPLAY_GRID_ROWS - 1; ++row) {
        m_board[row][0] = "|"; // Left border
        m_board[row][BOARD_DISPLAY_GRID_COLS - 1] = "|"; // Right border
    }

    // Inner horizontal lines (including top/bottom separators for coords)
    for (size_t row = 2; row < BOARD_DISPLAY_GRID_ROWS - 1; row += 4) { // Start at 2, jump by 4 (square height)
        for (size_t col = 3; col < BOARD_DISPLAY_GRID_COLS - 3; ++col) { // Horizontal line within the board area
            m_board[row][col] = "-";
        }
    }

    // Inner vertical lines (including left/right separators for coords)
    for (size_t col = 3; col < BOARD_DISPLAY_GRID_COLS - 3; col += 6) { // Start at 3, jump by 6 (square width)
        for (size_t row = 2; row < BOARD_DISPLAY_GRID_ROWS - 2; ++row) { // Vertical line within the board area
            m_board[row][col] = "|";
        }
    }

    // Intersections ('+') for inner grid lines
    for (size_t r = 2; r < BOARD_DISPLAY_GRID_ROWS - 2; r += 4) { // Horizontal lines
        for (size_t c = 3; c < BOARD_DISPLAY_GRID_COLS - 3; c += 6) { // Vertical lines
            m_board[r][c] = "+";
        }
    }

    // Special intersections for outer frame corners connecting to inner grid
    m_board[0][3] = "+"; // Top border, left inner separator
    m_board[0][BOARD_DISPLAY_GRID_COLS - 4] = "+"; // Top border, right inner separator
    m_board[BOARD_DISPLAY_GRID_ROWS - 1][3] = "+"; // Bottom border, left inner separator
    m_board[BOARD_DISPLAY_GRID_ROWS - 1][BOARD_DISPLAY_GRID_COLS - 4] = "+"; // Bottom border, right inner separator

    // Crosses within the main board grid
    for (size_t r = 3; r < BOARD_DISPLAY_GRID_ROWS - 1; r += BOARD_DISPLAY_HEIGHT_PER_SQUARE) {
        for (size_t c = 3; c < BOARD_DISPLAY_GRID_COLS - 1; c += BOARD_DISPLAY_WIDTH_PER_SQUARE) {
            if (m_board[r][c] == "-" || m_board[r][c] == "|") m_board[r][c] = "+";
        }
    }

    // Column numbers (1-8) at top and bottom
    for (size_t i = 0; i < BOARD_COLS_VISIBLE; ++i) {
        char digit = ('1' + i);
        size_t col_center_num = 6 + (i * BOARD_DISPLAY_WIDTH_PER_SQUARE);
        m_board[1][col_center_num] = string(1, digit); // Top row
        m_board[BOARD_DISPLAY_GRID_ROWS - 2][col_center_num] = string(1, digit); // Bottom row
    }

    // Row letters (A-H) at left and right
    for (size_t i = 0; i < BOARD_ROWS_VISIBLE; ++i) {
        char letter = ('A' + i);
        size_t row_center_letter = 4 + (i * BOARD_DISPLAY_HEIGHT_PER_SQUARE);
        m_board[row_center_letter][1] = string(1, letter); // Left column
        m_board[row_center_letter][BOARD_DISPLAY_GRID_COLS - 2] = string(1, letter); // Right column
    }
}

#else // non-Windows

void Chess::clear() const
{
	cout << "\033[2J\033[3J\033[H";
}

void Chess::setFrames()
{
    // Initialize entire m_board with spaces
    for (size_t row = 0; row < BOARD_DISPLAY_GRID_SIZE; ++row) {
        for (size_t col = 0; col < BOARD_DISPLAY_GRID_SIZE; ++col) {
            m_board[row][col] = " ";
        }
    }

    // Outer Frame Borders
    for (size_t col = 0; col < BOARD_DISPLAY_GRID_COLS; ++col) {
        m_board[0][col] = (col == 0 || col == BOARD_DISPLAY_GRID_COLS - 1) ? "+" : "-"; // Top border
        m_board[BOARD_DISPLAY_GRID_ROWS - 1][col] = (col == 0 || col == BOARD_DISPLAY_GRID_COLS - 1) ? "+" : "-"; // Bottom border
    }
    for (size_t row = 1; row < BOARD_DISPLAY_GRID_ROWS - 1; ++row) {
        m_board[row][0] = "|"; // Left border
        m_board[row][BOARD_DISPLAY_GRID_COLS - 1] = "|"; // Right border
    }

    // Inner horizontal lines (including top/bottom separators for coords)
    for (size_t row = 2; row < BOARD_DISPLAY_GRID_ROWS - 1; row += 4) {
        for (size_t col = 3; col < BOARD_DISPLAY_GRID_COLS - 3; ++col) {
            m_board[row][col] = "-";
        }
    }

    // Inner vertical lines (including left/right separators for coords)
    for (size_t col = 3; col < BOARD_DISPLAY_GRID_COLS - 3; col += 6) {
        for (size_t row = 2; row < BOARD_DISPLAY_GRID_ROWS - 2; ++row) {
            m_board[row][col] = "|";
        }
    }

    // Intersections ('+') for inner grid lines
    for (size_t r = 2; r < BOARD_DISPLAY_GRID_ROWS - 2; r += 4) {
        for (size_t c = 3; c < BOARD_DISPLAY_GRID_COLS - 3; c += 6) {
            m_board[r][c] = "+";
        }
    }

    // Special intersections for outer frame corners connecting to inner grid
    m_board[0][3] = "+";
    m_board[0][BOARD_DISPLAY_GRID_COLS - 4] = "+";
    m_board[BOARD_DISPLAY_GRID_ROWS - 1][3] = "+";
    m_board[BOARD_DISPLAY_GRID_ROWS - 1][BOARD_DISPLAY_GRID_COLS - 4] = "+";

    // Crosses within the main board grid
    for (size_t r = 3; r < BOARD_DISPLAY_GRID_ROWS - 1; r += BOARD_DISPLAY_HEIGHT_PER_SQUARE) {
        for (size_t c = 3; c < BOARD_DISPLAY_GRID_COLS - 1; c += BOARD_DISPLAY_WIDTH_PER_SQUARE) {
            if (m_board[r][c] == "-" || m_board[r][c] == "|") m_board[r][c] = "+";
        }
    }

    // Column numbers (1-8) at top and bottom
    for (size_t i = 0; i < BOARD_COLS_VISIBLE; ++i) {
        char digit = ('1' + i);
        size_t col_center_num = 6 + (i * BOARD_DISPLAY_WIDTH_PER_SQUARE);
        m_board[1][col_center_num] = string(1, digit);
        m_board[BOARD_DISPLAY_GRID_ROWS - 2][col_center_num] = string(1, digit);
    }

    // Row letters (A-H) at left and right
    for (size_t i = 0; i < BOARD_ROWS_VISIBLE; ++i) {
        char letter = ('A' + i);
        size_t row_center_letter = 4 + (i * BOARD_DISPLAY_HEIGHT_PER_SQUARE);
        m_board[row_center_letter][1] = string(1, letter);
        m_board[row_center_letter][BOARD_DISPLAY_GRID_COLS - 2] = string(1, letter);
    }
}
#endif // non-Windows


void Chess::syncBoardStringWithBoard()
{
    if (!m_gameEngine) {
        // Clear all piece display cells if no engine
        for(int r_board = 0; r_board < BOARD_ROWS_VISIBLE; ++r_board) {
            for(int c_board = 0; c_board < BOARD_COLS_VISIBLE; ++c_board) {
                size_t r_content_start = 3 + (r_board * BOARD_DISPLAY_HEIGHT_PER_SQUARE); // Row for piece content
                size_t c_content_start = 4 + (c_board * BOARD_DISPLAY_WIDTH_PER_SQUARE); // Col for piece symbol

                // Clear the 5x3 content area for this square
                for(int r_offset = 0; r_offset < 3; ++r_offset) {
                    for(int c_offset = 0; c_offset < 5; ++c_offset) { // 5-char visual width per square
                        m_board[r_content_start + r_offset][c_content_start + c_offset] = " ";
                    }
                }
            }
        }
        return;
    }

    const Board& b = m_gameEngine->currentBoard();
    for (int r = 0; r < BOARD_ROWS_VISIBLE; ++r) {
        for (int c = 0; c < BOARD_COLS_VISIBLE; ++c) {
            size_t r_content_start = 3 + (r * BOARD_DISPLAY_HEIGHT_PER_SQUARE); // Row for piece content
            // FIX TYPO HERE: Use BOARD_DISPLAY_WIDTH_PER_SQUARE
            size_t c_content_start = 4 + (c * BOARD_DISPLAY_WIDTH_PER_SQUARE); // Col for piece symbol (center-ish)


            // IMPORTANT: Clear the *entire* 5-char content area for this square first
            for(int r_offset = 0; r_offset < 3; ++r_offset) {
                for(int c_offset = 0; c_offset < 5; ++c_offset) {
                    m_board[r_content_start + r_offset][c_content_start + c_offset] = " ";
                }
            }

            if (const Piece* p = b.getPiece(r, c)) {
                const std::string& symbol = p->getSymbol();
                m_board[r_content_start + 1][c_content_start + 1] = symbol;
            }
        }
    }
}


void Chess::show() const
{
    for (size_t row = 0; row < BOARD_DISPLAY_GRID_ROWS; ++row) {
        for (size_t col = 0; col < BOARD_DISPLAY_GRID_COLS; ++col) {
            const std::string& cellContent = m_board[row][col];
            bool isPieceSymbolStart = ((row - 4) % BOARD_DISPLAY_HEIGHT_PER_SQUARE == 1) && // Piece symbol row (row+1 of 3 content rows)
                                      ((col - 6) % BOARD_DISPLAY_WIDTH_PER_SQUARE == 0); // Piece symbol column (first col of the 5-char content)
            // If it's a piece symbol start and it's not empty/space
            if (isPieceSymbolStart && !cellContent.empty() && cellContent != " ") {
                // Check if it's a piece symbol (you might already have this from the map)
                bool isWhitePiece = (cellContent == Pieces::WHITE_KING || cellContent == Pieces::WHITE_QUEEN ||
                                     cellContent == Pieces::WHITE_ROOK || cellContent == Pieces::WHITE_BISHOP ||
                                     cellContent == Pieces::WHITE_KNIGHT || cellContent == Pieces::WHITE_PAWN);
                bool isBlackPiece = (cellContent == Pieces::BLACK_KING || cellContent == Pieces::BLACK_QUEEN ||
                                     cellContent == Pieces::BLACK_ROOK || cellContent == Pieces::BLACK_BISHOP ||
                                     cellContent == Pieces::BLACK_KNIGHT || cellContent == Pieces::BLACK_PAWN);

                if (isWhitePiece) {
                    cout << WHITE_PIECE << BOLD << cellContent << RESET;
                    col++; 
                } else if (isBlackPiece) {
                    cout << BLACK_PIECE << BOLD << cellContent << RESET;
                    col++; // Same for black pieces
                } else {
                    // Fallback for unexpected non-piece content at a piece symbol position
                    cout << cellContent;
                }
            }
            // All other cells (borders, coordinates, padding spaces, or second half of a wide char that was already printed)
            else {
                // Apply colors for borders, coordinates, grid lines
                if (row == 0 || row == BOARD_DISPLAY_GRID_ROWS - 1 ||
                    col == 0 || col == BOARD_DISPLAY_GRID_COLS - 1 ||
                    (row >= 2 && row < BOARD_DISPLAY_GRID_ROWS - 1 && col >=3 && col < BOARD_DISPLAY_GRID_COLS - 1 && (cellContent == "-" || cellContent == "|" || cellContent == "+"))
                   ) {
                    cout << BOARD_FRAME << cellContent << RESET;
                } else if ((row == 1 || row == BOARD_DISPLAY_GRID_ROWS - 2) && (cellContent != " ")) { // Numbers
                     cout << COORD_COLOR << cellContent << RESET;
                } else if ((col == 1 || col == BOARD_DISPLAY_GRID_COLS - 2) && (cellContent != " ")) { // Letters
                     cout << COORD_COLOR << cellContent << RESET;
                } else {
                    // This will handle normal empty spaces and the second visual char of Unicode (which is technically empty in the array)
                    cout << cellContent;
                }
            }
        }
        cout << endl; // Newline after each row
    }
}


void Chess::displayBoard() const
{
    clear();

    cout << INFO_COLOR << "╔═══════════════════════════════╗" << RESET << endl;
    cout << INFO_COLOR << "║" << RESET;
    cout << YELLOW << "       ♔  CHESS GAME ♚         " << RESET;
    cout << INFO_COLOR << "║" << RESET << endl;
    cout << INFO_COLOR << "╚═══════════════════════════════╝" << RESET << endl;
    cout << endl;

    show();

    if (!m_msg.empty() && m_msg != "\n") {
        cout << CYAN << "▶ " << m_msg << RESET;
    }

    if (!m_errorMsg.empty() && m_errorMsg != "\n") {
        cout << RED << "✗ " << m_errorMsg << RESET;
    }

    if (!m_hint.empty() && m_hint != "\n") {
        cout << HINT_COLOR << "💡 " << m_hint << RESET;
    }
}


void Chess::showAskInput() const
{
    if (m_gameEngine && m_gameEngine->whiteToMove()) {
        cout << YELLOW << "♙ Player 1 " << RESET
             << "(White - " << CYAN << "lowercase" << RESET << ") "
             << ORANGE << "➤ " << RESET;
    } else if (m_gameEngine) {
        cout << PURPLE << "♟ Player 2 " << RESET
             << "(Black - " << CYAN << "UPPERCASE" << RESET << ") "
             << ORANGE << "➤ " << RESET;
    } else {
        cout << "Player (unknown turn) ➤ ";
    }
}


bool Chess::isSame() const
{
	return ((m_input[0] == m_input[2]) && (m_input[1] == m_input[3]));
}

bool Chess::isValid() const
{
    return ((('A' <= m_input[0] && m_input[0] <= 'H') || ('a' <= m_input[0] && m_input[0] <= 'h')) &&
            ('1' <= m_input[1] && m_input[1] <= '8') &&
            (('A' <= m_input[2] && m_input[2] <= 'H') || ('a' <= m_input[2] && m_input[2] <= 'h')) &&
            ('1' <= m_input[3] && m_input[3] <= '8'));
}

bool Chess::isExit() const
{
	return ((m_input == "exit") || (m_input == "quit") || (m_input == "EXIT") || (m_input == "QUIT"));
}


void Chess::excute()
{
    syncBoardStringWithBoard();
}


void Chess::doTurn()
{
    m_errorMsg = "\n";
    m_msg = "\n";
    m_hint = "\n";

    switch (m_codeResponse)
    {
    case 11: m_msg = "There is no piece at the source position or invalid coordinates.\n"; break;
    case 12: m_msg = "That piece belongs to your opponent.\n"; break;
    case 13: m_msg = "One of your pieces is already at the destination.\n"; break;
    case 21: m_msg = "Illegal movement for that piece.\n"; break;
    case 31: m_msg = "This movement would leave you in check!\n"; break;
    case 41:
    {
        excute();

        if (m_gameEngine) {
            int hintSearchDepth = m_gameEngine->getSearchDepth();
            auto recs = AI::findBestMoves(m_gameEngine->currentBoard(), m_gameEngine->whiteToMove(), 1, 1, hintSearchDepth);

            if (!recs.empty()) {
                std::string hint = recs.front().toString();
                m_hint = "Hint: " + hint + '\n';
            } else {
                m_hint.clear();
            }
        }
        m_msg = BOLD + "CHECK!" + RESET + " Your opponent's king is under attack\n";
        break;
    }
    case 42:
    {
        excute();

        if (m_gameEngine) {
            int hintSearchDepth = m_gameEngine->getSearchDepth();
            auto recs = AI::findBestMoves(m_gameEngine->currentBoard(), m_gameEngine->whiteToMove(), 1, 1, hintSearchDepth);

            if (!recs.empty()) {
                std::string hint = recs.front().toString();
                m_hint = "Hint: " + hint + '\n';
            } else {
                m_hint.clear();
            }
        }
        m_msg = "Move successful\n";
        break;
    }
    case 99:
    case 98:
        m_msg = "\n";
        excute();
        break;
    default:
        m_msg = "\n";
        break;
    }
}

Chess::Chess(const string& start, GameEngine* engine)
	: m_boardString(start), m_codeResponse(-1), m_gameEngine(engine)
{
	setFrames();
	if (m_gameEngine) {
	    syncBoardStringWithBoard();
	}
}

string Chess::getInput()
{
    doTurn();
    displayBoard();

    showAskInput();
    cin >> m_input;
    if (isExit())
        return "exit";

    while (!isValid() || isSame())
    {
        if (!isValid())
            m_errorMsg = "Invalid input! Use format like: a2a4\n";
        else
            m_errorMsg = "Source and destination cannot be the same!\n";

        doTurn();
        displayBoard();

        showAskInput();
        cin >> m_input;
        if (isExit())
            return "exit";
    }

    if (m_input != "exit")
    {
        m_input[0] = std::tolower(static_cast<unsigned char>(m_input[0]));
        m_input[2] = std::tolower(static_cast<unsigned char>(m_input[2]));
    }

    return m_input;
}

void Chess::setCodeResponse(int codeResponse)
{
	if (((11 <= codeResponse) && (codeResponse <= 13)) ||
		((21 == codeResponse) || (codeResponse == 31)) ||
		((41 == codeResponse) || (codeResponse == 42)) ||
        (codeResponse == 98) || (codeResponse == 99) )
	{
		m_codeResponse = codeResponse;
	} else {
	    m_codeResponse = -1;
	}
}