// Chess/src/Chess.cpp
#include "Chess.h"
#include "GameEngine.h"
#include "AI/BestMoveFinder.h"
#include "Utils/Colors.h"

#include <iostream>
#include <string>
#include <cctype>
#include <iomanip> // For std::setw if needed for centering, but manual padding is used

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

    // Inner Board Grid Lines (horizontal and vertical)
    // Horizontal lines:
    // Rows 3, 7, 11, 15, 19, 23, 27, 31 (between board rows)
    // Rows 2 and BOARD_DISPLAY_GRID_ROWS-3 (between coords and board)
    for (size_t row_idx = 2; row_idx < BOARD_DISPLAY_GRID_ROWS - 1; row_idx += 4) {
        if (row_idx == 2 || row_idx == BOARD_DISPLAY_GRID_ROWS - 3) { // Coords separators
            for (size_t col = 3; col < BOARD_DISPLAY_GRID_COLS - 3; ++col) {
                m_board[row_idx][col] = "-";
            }
        } else { // Inner board separators
            for (size_t col = 3; col < BOARD_6_COL_END - 3; ++col) { // Inner board content width
                m_board[row_idx][col] = "-"; // Horizontal part
            }
        }
    }

    // Vertical lines:
    // Cols 3, 6, 10, 14, ..., 46 (between coords and board)
    // And inner board cols
    for (size_t col_idx = 3; col_idx < BOARD_DISPLAY_GRID_COLS - 1; col_idx += 3) { // Adjusted to match 3 char width of coords
        if (col_idx == 3 || col_idx == BOARD_DISPLAY_GRID_COLS - 4) { // Coords separators
            for (size_t row = 2; row < BOARD_DISPLAY_GRID_ROWS - 2; ++row) {
                m_board[row][col_idx] = "|";
            }
        } else { // Inner board separators
            for (size_t row = 2; row < BOARD_DISPLAY_GRID_ROWS - 2; ++row) {
                m_board[row][col_idx] = "|";
            }
        }
    }
    
    // Intersection points for inner grid and coord frames
    for (size_t r = 2; r < BOARD_DISPLAY_GRID_ROWS - 2; r += 4) {
        for (size_t c = 3; c < BOARD_DISPLAY_GRID_COLS - 3; c += 6) { // Every 6th column for '+'
            m_board[r][c] = "+";
        }
    }
    // Vertical bars for inner grid within coordinates area (simplified for now)
    m_board[1][3] = "|"; // Top coordinates, left separator
    m_board[BOARD_DISPLAY_GRID_ROWS - 2][3] = "|"; // Bottom coordinates, left separator
    m_board[1][BOARD_DISPLAY_GRID_COLS - 4] = "|"; // Top coordinates, right separator
    m_board[BOARD_DISPLAY_GRID_ROWS - 2][BOARD_DISPLAY_GRID_COLS - 4] = "|"; // Bottom coordinates, right separator

    // Crosses within the main board grid
    for (size_t r = 3; r < BOARD_DISPLAY_GRID_ROWS - 1; r += BOARD_DISPLAY_HEIGHT_PER_SQUARE) {
        for (size_t c = 3; c < BOARD_DISPLAY_GRID_COLS - 1; c += BOARD_DISPLAY_WIDTH_PER_SQUARE) {
            if (m_board[r][c] == "-") m_board[r][c] = "+";
            if (m_board[r][c] == "|") m_board[r][c] = "+";
        }
    }
    
    // Center positions for numbers and letters (relative to outer frame)
    // Top/Bottom numbers (1-8)
    for (size_t i = 0; i < BOARD_COLS_VISIBLE; ++i) {
        char digit = ('1' + i);
        size_t col_center_num = 6 + (i * BOARD_DISPLAY_WIDTH_PER_SQUARE); // 6: start of first square + (i*6)
        m_board[1][col_center_num] = string(1, digit); // Top row
        m_board[BOARD_DISPLAY_GRID_ROWS - 2][col_center_num] = string(1, digit); // Bottom row
    }

    // Left/Right letters (A-H)
    for (size_t i = 0; i < BOARD_ROWS_VISIBLE; ++i) {
        char letter = ('A' + i);
        size_t row_center_letter = 4 + (i * BOARD_DISPLAY_HEIGHT_PER_SQUARE); // 4: start of first square content + (i*4)
        m_board[row_center_letter][1] = string(1, letter); // Left column
        m_board[row_center_letter][BOARD_DISPLAY_GRID_COLS - 2] = string(1, letter); // Right column
    }
}

void Chess::setPieces()
{
    // Logic moved to syncBoardStringWithBoard. This function is essentially empty.
}

#else // non-Windows (Assuming similar visual requirements for non-Windows)

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

    // Inner Board Grid Lines (horizontal and vertical)
    for (size_t row_idx = 2; row_idx < BOARD_DISPLAY_GRID_ROWS - 1; row_idx += 4) {
        if (row_idx == 2 || row_idx == BOARD_DISPLAY_GRID_ROWS - 3) {
            for (size_t col = 3; col < BOARD_DISPLAY_GRID_COLS - 3; ++col) {
                m_board[row_idx][col] = "-";
            }
        } else {
            for (size_t col = 3; col < BOARD_DISPLAY_GRID_COLS - 3; ++col) {
                m_board[row_idx][col] = "-";
            }
        }
    }

    for (size_t col_idx = 3; col_idx < BOARD_DISPLAY_GRID_COLS - 1; col_idx += 6) {
        if (col_idx == 3 || col_idx == BOARD_DISPLAY_GRID_COLS - 4) {
            for (size_t row = 2; row < BOARD_DISPLAY_GRID_ROWS - 2; ++row) {
                m_board[row][col_idx] = "|";
            }
        } else {
            for (size_t row = 2; row < BOARD_DISPLAY_GRID_ROWS - 2; ++row) {
                m_board[row][col_idx] = "|";
            }
        }
    }

    // Intersections ('+') of inner grid lines
    for (size_t r = 2; r < BOARD_DISPLAY_GRID_ROWS - 2; r += 4) {
        for (size_t c = 3; c < BOARD_DISPLAY_GRID_COLS - 3; c += 6) {
            m_board[r][c] = "+";
        }
    }

    // Vertical bars for inner grid within coordinates area
    m_board[1][3] = "|";
    m_board[BOARD_DISPLAY_GRID_ROWS - 2][3] = "|";
    m_board[1][BOARD_DISPLAY_GRID_COLS - 4] = "|";
    m_board[BOARD_DISPLAY_GRID_ROWS - 2][BOARD_DISPLAY_GRID_COLS - 4] = "|";

    // Crosses within the main board grid
    for (size_t r = 3; r < BOARD_DISPLAY_GRID_ROWS - 1; r += BOARD_DISPLAY_HEIGHT_PER_SQUARE) {
        for (size_t c = 3; c < BOARD_DISPLAY_GRID_COLS - 1; c += BOARD_DISPLAY_WIDTH_PER_SQUARE) {
            if (m_board[r][c] == "-") m_board[r][c] = "+";
            if (m_board[r][c] == "|") m_board[r][c] = "+";
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

void Chess::setPieces()
{
    // Logic moved to syncBoardStringWithBoard. This function is essentially empty.
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
                    for(int c_offset = 0; c_offset < 5; ++c_offset) {
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
            size_t c_content_start = 4 + (c * BOARD_DISPLAY_WIDTH_PER_SQUARE); // Col for piece symbol (center-ish)

            if (const Piece* p = b.getPiece(r, c)) {
                // Piece symbol, centered within a 5-char visual width
                // For a 2-char wide Unicode piece in a 5-char space:
                // [ ] [P] [I] [ ] [ ] => P at c_content_start + 1, I at c_content_start + 2
                // We'll place the piece at c_content_start + 1 and let it draw two chars.
                // The third char is just a space from clear.
                m_board[r_content_start + 1][c_content_start + 1] = p->getSymbol(); // Piece row: 1 of 3
            } else {
                // Empty squares, clear the content area
                for(int r_offset = 0; r_offset < 3; ++r_offset) {
                    for(int c_offset = 0; c_offset < 5; ++c_offset) {
                        m_board[r_content_start + r_offset][c_content_start + c_offset] = " ";
                    }
                }
            }
        }
    }
}


void Chess::show() const
{
    // Iterate through the entire m_board array and print
    for (size_t row = 0; row < BOARD_DISPLAY_GRID_ROWS; ++row) {
        for (size_t col = 0; col < BOARD_DISPLAY_GRID_COLS; ++col) {
            const std::string& cellContent = m_board[row][col];

            // Apply colors based on the type of content at this specific cell
            // Outer borders
            if (row == 0 || row == BOARD_DISPLAY_GRID_ROWS - 1 ||
                col == 0 || col == BOARD_DISPLAY_GRID_COLS - 1)
            {
                cout << BOARD_FRAME << cellContent << RESET;
            }
            // Top/Bottom coordinate rows (Row 1 and BOARD_DISPLAY_GRID_ROWS-2)
            else if (row == 1 || row == BOARD_DISPLAY_GRID_ROWS - 2) {
                // Column numbers are at BOARD_DISPLAY_WIDTH_PER_SQUARE + 1 + (i * BOARD_DISPLAY_WIDTH_PER_SQUARE)
                // For example, 1 is at col 6, 2 at col 12, etc. (index 6, 12, ...)
                if (col >= 6 && (col - 6) % BOARD_DISPLAY_WIDTH_PER_SQUARE == 0 && col < BOARD_DISPLAY_GRID_COLS - 6) {
                    cout << COORD_COLOR << cellContent << RESET;
                } else {
                    cout << BOARD_FRAME << cellContent << RESET;
                }
            }
            // Left/Right coordinate columns (Col 1 and BOARD_DISPLAY_GRID_COLS-2)
            else if (col == 1 || col == BOARD_DISPLAY_GRID_COLS - 2) {
                // Row letters are at 4 + (i * BOARD_DISPLAY_HEIGHT_PER_SQUARE) (index 4, 8, ...)
                if (row >= 4 && (row - 4) % BOARD_DISPLAY_HEIGHT_PER_SQUARE == 0 && row < BOARD_DISPLAY_GRID_ROWS - 4) {
                    cout << COORD_COLOR << cellContent << RESET;
                } else {
                    cout << BOARD_FRAME << cellContent << RESET;
                }
            }
            // Inner grid lines and intersections (these are '-' or '|' or '+')
            else if (m_board[row][col] == "-" || m_board[row][col] == "|" || m_board[row][col] == "+") {
                cout << BOARD_FRAME << cellContent << RESET;
            }
            // Piece or empty square content
            else {
                // Determine if this is the row and column where a piece symbol would be placed
                // Piece row: 4 + (r_board * 4) + 1 (middle of the 3 content rows)
                // Piece col: 4 + (c_board * 6) + 1 (left padding 1, piece 2, right padding 2)
                bool isPieceSymbolContentCell = (row >=4 && row <= (BOARD_DISPLAY_GRID_ROWS - 5)) && // within piece rows
                                                (col >=6 && col <= (BOARD_DISPLAY_GRID_COLS - 7)) && // within piece cols
                                                ((row - 4) % BOARD_DISPLAY_HEIGHT_PER_SQUARE == 1) && // This is the piece symbol row within the 4-row square
                                                ((col - 6) % BOARD_DISPLAY_WIDTH_PER_SQUARE == 0); // This is the piece symbol column (start of 2 chars)

                if (isPieceSymbolContentCell) {
                    const std::string& pieceSymbol = m_board[row][col]; // Get the actual symbol
                    if (pieceSymbol == Pieces::WHITE_KING || pieceSymbol == Pieces::WHITE_QUEEN ||
                        pieceSymbol == Pieces::WHITE_ROOK || pieceSymbol == Pieces::WHITE_BISHOP ||
                        pieceSymbol == Pieces::WHITE_KNIGHT || pieceSymbol == Pieces::WHITE_PAWN)
                    {
                        cout << WHITE_PIECE << BOLD << pieceSymbol << RESET;
                    }
                    else if (pieceSymbol == Pieces::BLACK_KING || pieceSymbol == Pieces::BLACK_QUEEN ||
                             pieceSymbol == Pieces::BLACK_ROOK || pieceSymbol == Pieces::BLACK_BISHOP ||
                             pieceSymbol == Pieces::BLACK_KNIGHT || pieceSymbol == Pieces::BLACK_PAWN)
                    {
                        cout << BLACK_PIECE << BOLD << pieceSymbol << RESET;
                    }
                    else { // Should be a space for an empty symbol cell
                        cout << cellContent; // Prints " " (empty symbol cell)
                    }
                } else { // This is a padding cell within the square content (e.g., "   ")
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
    cout << YELLOW << "       ♔ CHESS GAME ♚        " << RESET;
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
            auto recs = AI::findBestMoves(m_gameEngine->currentBoard(), m_gameEngine->whiteToMove(), 1, hintSearchDepth);

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
            auto recs = AI::findBestMoves(m_gameEngine->currentBoard(), m_gameEngine->whiteToMove(), 1, hintSearchDepth);

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