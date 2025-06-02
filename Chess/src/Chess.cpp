// Chess.cpp
#include "Chess.h"
#include <iostream>
#include <string>

using namespace std;

#ifdef _WIN32

// Clear the console on Windows
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

// Build the ASCII‐art frame on Windows
void Chess::setFrames()
{
    for (size_t row = 0; row < _SIZE; ++row)
        for (size_t col = 0; col < _SIZE; ++col)
            m_board[row][col] = 32; // space

    m_board[0][0] = 201;   m_board[0][20] = 187;
    m_board[20][0] = 200;  m_board[20][20] = 188;

    for (size_t i = 1; i < 20; ++i)
    {
        m_board[0][i] = 205;    // ─
        m_board[20][i] = 205;   // ─
        m_board[i][0] = 186;    // │
        m_board[i][20] = 186;   // │
    }

    m_board[2][2] = 218;    m_board[2][18] = 191;
    m_board[18][2] = 192;   m_board[18][18] = 217;

    for (size_t i = 4; i < 17; i += 2)
    {
        m_board[2][i] = 194;   // ┬
        m_board[18][i] = 193;  // ┴
        m_board[i][2] = 195;   // ├
        m_board[i][18] = 180;  // ┤
    }

    for (size_t i = 2; i < 19; i += 2)
        for (size_t j = 3; j < 19; j += 2)
            m_board[i][j] = 196;  // ─

    for (size_t i = 3; i < 18; i += 2)
        for (size_t j = 2; j < 19; j += 2)
            m_board[i][j] = 179;  // │

    for (size_t i = 4; i < 17; i += 2)
        for (size_t j = 4; j < 17; j += 2)
            m_board[i][j] = 197;  // ┼

    // Re‐set intersections explicitly (just in case)
    for (size_t i = 4; i < 17; i += 2)
    {
        m_board[2][i] = 194;
        m_board[18][i] = 193;
        m_board[i][2] = 195;
        m_board[i][18] = 180;
    }

    // Rank labels (1…8) on left/right
    for (size_t i = 3, t = 0; i < 19; i += 2, ++t)
        m_board[1][i] = m_board[19][i] = char('1' + t);

    // File labels (A…H) on top/bottom
    for (size_t i = 3, t = 0; i < 19; i += 2, ++t)
        m_board[i][1] = m_board[i][19] = char('A' + t);
}

// Place pieces onto the ASCII grid (Windows)
void Chess::setPieces()
{
    for (size_t row = 0, t = 0; row < 8; ++row)
        for (size_t col = 0; col < 8; ++col, ++t)
            m_board[3 + (row * 2)][3 + (col * 2)] =
                (m_boardString[t] == '#') ? 32 : m_boardString[t];
}

#else  // non‐Windows (Linux/macOS)

void Chess::clear() const
{
    // ANSI codes: clear screen + move cursor to home
    cout << "\033[2J\033[3J\033[H";
}

void Chess::setFrames()
{
    for (size_t row = 0; row < _SIZE; ++row)
        for (size_t col = 0; col < _SIZE; ++col)
            m_board[row][col] = ' ';

    m_board[0][0] = '+';
    m_board[0][20] = '+';
    m_board[20][0] = '+';
    m_board[20][20] = '+';

    for (size_t i = 1; i < 20; ++i)
    {
        m_board[0][i] = '-';
        m_board[20][i] = '-';
        m_board[i][0] = '|';
        m_board[i][20] = '|';
    }

    m_board[2][2] = '+';
    m_board[2][18] = '+';
    m_board[18][2] = '+';
    m_board[18][18] = '+';

    for (size_t i = 4; i < 17; i += 2)
    {
        m_board[2][i] = '+';
        m_board[18][i] = '+';
        m_board[i][2] = '+';
        m_board[i][18] = '+';
    }

    for (size_t i = 2; i < 19; i += 2)
        for (size_t j = 3; j < 19; j += 2)
            m_board[i][j] = '-';

    for (size_t i = 3; i < 18; i += 2)
        for (size_t j = 2; j < 19; j += 2)
            m_board[i][j] = '|';

    for (size_t i = 4; i < 17; i += 2)
        for (size_t j = 4; j < 17; j += 2)
            m_board[i][j] = '+';

    for (size_t i = 4; i < 17; i += 2)
        m_board[2][i] = '+';
    for (size_t i = 4; i < 17; i += 2)
        m_board[18][i] = '+';
    for (size_t i = 4; i < 17; i += 2)
        m_board[i][2] = '+';
    for (size_t i = 4; i < 17; i += 2)
        m_board[i][18] = '+';

    for (size_t i = 3, t = 0; i < 19; i += 2, ++t)
        m_board[1][i] = m_board[19][i] = char('1' + t);

    for (size_t i = 3, t = 0; i < 19; i += 2, ++t)
        m_board[i][1] = m_board[i][19] = char('A' + t);
}

void Chess::setPieces()
{
    for (size_t row = 0, t = 0; row < 8; ++row)
        for (size_t col = 0; col < 8; ++col, ++t)
            m_board[3 + (row * 2)][3 + (col * 2)] =
                    (m_boardString[t] == '#') ? ' ' : m_boardString[t];
}

#endif // _WIN32

// Print only the ASCII grid
void Chess::show() const
{
    for (size_t row = 0; row < _SIZE; ++row)
    {
        for (size_t col = 0; col < _SIZE; ++col)
            cout << m_board[row][col];
        cout << '\n';
    }
}

// Clear + draw board + show messages
void Chess::displayBoard() const
{
    clear();
    show();
    cout << m_msg << m_errorMsg;
}

// Show prompt for whose turn it is
void Chess::showAskInput() const
{
    if (m_turn)
        cout << "Player 1 (White - uppercase) >> ";
    else
        cout << "Player 2 (Black - lowercase)   >> ";
}

// Return true if source and dest squares are identical
bool Chess::isSame() const
{
    return (m_input[0] == m_input[2] && m_input[1] == m_input[3]);
}

// Validate that each character of m_input is within a–h and 1–8
bool Chess::isValid() const
{
    return (
            ((m_input[0] >= 'A' && m_input[0] <= 'H') || (m_input[0] >= 'a' && m_input[0] <= 'h'))
            && ((m_input[1] >= '1' && m_input[1] <= '8'))
            && ((m_input[2] >= 'A' && m_input[2] <= 'H') || (m_input[2] >= 'a' && m_input[2] <= 'h'))
            && ((m_input[3] >= '1' && m_input[3] <= '8'))
    );
}

// Return true if m_input equals “exit” or “quit” (any case)
bool Chess::isExit() const
{
    return (
            m_input == "exit" || m_input == "quit"
            || m_input == "EXIT" || m_input == "QUIT"
    );
}

// Interpret the integer code returned by logic.movePiece(...):
//   - If code < 0: no move attempted yet.
//   - If code is 1..7: “illegal” in various ways; show an error.
//   - Otherwise (>=10 or whatever): treat as “successful” and flip turn.
void Chess::doTurn()
{
    m_errorMsg = "\n";
    switch (m_codeResponse)
    {
        case -1:
            m_msg = "\n";  // no move yet
            break;

        case  1:
            m_msg = "Illegal format. Use files a–h and ranks 1–8.\n";
            break;
        case  2:
            m_msg = "Out of bounds. Use files a–h, ranks 1–8.\n";
            break;
        case  3:
            m_msg = "No piece at source.\n";
            break;
        case  4:
            m_msg = "That piece belongs to your opponent.\n";
            break;
        case  5:
        case  6:
        case  7:
            m_msg = "Illegal move for that piece.\n";
            break;
        default:
            // Any code not 1..7 is considered “OK, flip turn”
            m_turn = !m_turn;
            m_msg = "Move executed.\n";
            break;
    }
}

// Constructor: store the initial 64‐char string and draw the board once
Chess::Chess(const string& start)
        : m_boardString(start),
          m_codeResponse(-1),
          m_turn(true)             // White starts
{
    setFrames();
    setPieces();
}

// Prompt the user; return “exit” if they choose to quit
string Chess::getInput()
{
    static bool first = true;
    if (first) {
        first = false;
    } else {
        // After the first call, we interpret the previous codeResponse
        doTurn();
    }

    displayBoard();
    showAskInput();

    cin >> m_input;
    if (isExit()) return "exit";

    // Validate length, alphanumeric, and distinct‐square
    while (!isValid() || isSame()) {
        if (!isValid())
            m_errorMsg = "Invalid input! Use file a–h and rank 1–8.\n";
        else
            m_errorMsg = "Source and destination are the same!\n";

        displayBoard();
        showAskInput();
        cin >> m_input;
        if (isExit()) return "exit";
    }

    // Convert uppercase file letters to lowercase
    if (m_input[0] >= 'A' && m_input[0] <= 'H')
        m_input[0] = char(m_input[0] - 'A' + 'a');
    if (m_input[2] >= 'A' && m_input[2] <= 'H')
        m_input[2] = char(m_input[2] - 'A' + 'a');

    return m_input;
}
