// Chess.h
#ifndef CHESS_H
#define CHESS_H

#include <string>

class Chess {
private:
    static constexpr size_t _SIZE = 21;   // ASCII‐art board is 21×21 chars

    char         m_board[_SIZE][_SIZE];   // the ASCII grid
    std::string  m_boardString;           // 64‐char “raw” board (shared with logic only via main)
    int          m_codeResponse;          // the code returned by logic.movePiece(...)
    bool         m_turn;                  // true = White’s turn; false = Black’s

    std::string  m_msg;                   // e.g. “Move executed.” or error text
    std::string  m_errorMsg;              // e.g. “Illegal move!” text
    std::string  m_input;                 // last user input, e.g. “e2e4”

public:
    // Construct with an initial 64‐char string
    Chess(const std::string& start);

    // Draw the board‐frame (Unicode/ASCII) and place pieces
    void setFrames();
    void setPieces();

    // Clear + redraw helpers
    void clear() const;
    void show() const;
    void displayBoard() const;
    void showAskInput() const;

    // Input validation
    bool isSame() const;
    bool isValid() const;
    bool isExit() const;

    // Called by main() after logic.movePiece(…) to set the new 64‐char string:
    void setBoardString(const std::string& s) {
        m_boardString = s;
    }
    // Called by main() to inform Chess what code logic.movePiece produced:
    void setCodeResponse(int codeResponse) {
        m_codeResponse = codeResponse;
    }

    // Interpret m_codeResponse: flip turn (or set m_msg) accordingly
    void doTurn();

    // Prompt user until they enter a valid “a2a4”‐style move (or “exit”)
    std::string getInput();
};

#endif // CHESS_H
