#pragma once
#include <iostream>
#ifdef _WIN32
#include <Windows.h>
#endif
#include <string>

class GameEngine;

using std::cout;
using std::cin;
using std::endl;
using std::string;

// Board display dimensions for aesthetic square cells and centering
// Each board square will be visually 5 chars wide (for content) x 3 lines high (for content)
// Including frame lines: a square is 6 chars wide x 4 lines high
const int BOARD_DISPLAY_WIDTH_PER_SQUARE = 6;
const int BOARD_DISPLAY_HEIGHT_PER_SQUARE = 4;

const int BOARD_COLS_VISIBLE = 8;
const int BOARD_ROWS_VISIBLE = 8;

// Total display grid dimensions calculated from visual requirements
const int BOARD_DISPLAY_GRID_COLS = 1 + // Left outer border
                                    1 + // Left coord padding
                                    1 + // Left coord (A-H)
                                    1 + // Left coord-board separator
                                    (BOARD_COLS_VISIBLE * BOARD_DISPLAY_WIDTH_PER_SQUARE) + // 8 squares * 6 chars/square
                                    1 + // Right coord-board separator
                                    1 + // Right coord (A-H)
                                    1 + // Right coord padding
                                    1; // Right outer border
// This resolves to 1+1+1+1 + (8*6) + 1+1+1+1 = 4 + 48 + 4 = 56.

const int BOARD_DISPLAY_GRID_ROWS = 1 + // Top outer border
                                    1 + // Top row for numbers
                                    1 + // Top separator line
                                    (BOARD_ROWS_VISIBLE * BOARD_DISPLAY_HEIGHT_PER_SQUARE) + // 8 squares * 4 lines/square
                                    1 + // Bottom separator line
                                    1 + // Bottom row for numbers
                                    1; // Bottom outer border
// This resolves to 1+1+1 + (8*4) + 1+1+1 = 3 + 32 + 3 = 38.

// Use the largest dimension for the square array (which is COLS: 56)
const int BOARD_DISPLAY_GRID_SIZE = BOARD_DISPLAY_GRID_COLS; // Set to 56, matching BOARD_DISPLAY_GRID_COLS

class Chess {
    std::string m_board[BOARD_DISPLAY_GRID_SIZE][BOARD_DISPLAY_GRID_SIZE]; // Max dimension
    string m_boardString;
    string m_input;
    string m_msg = "\n";
    string m_errorMsg = "\n";
    string m_hint = "\n";
    int m_codeResponse;

    GameEngine* m_gameEngine;

    void clear() const;
    void setFrames();
    void syncBoardStringWithBoard();
    void show() const;
public:
    void displayBoard() const;
private:
    void showAskInput() const;
    bool isSame() const;
    bool isValid() const;
    bool isExit() const;
    void excute();
    void doTurn();

public:
    Chess(const string& start = "RNBQKBNRPPPPPPPP################################pppppppprnbqkbnr",
          GameEngine* engine = nullptr);
    Chess(const Chess&)=delete;
    Chess& operator=(const Chess&) = delete;

    string getInput();
    void setCodeResponse(int codeResponse);
};