#pragma once
#include <iostream>
#ifdef _WIN32
#include <Windows.h>
#endif

#include <string>
#include "Rook.h"
#include "Bishop.h"
#include "Queen.h"
#include "King.h"
#include "Knight.h"
#include "Pawn.h"

#include "PriorityQueue.h"
#include "Piece.h"
#include "ChessAI.h"

using std::cout;
using std::cin; 
using std::endl;
using std::string; 

const int _SIZE = 21;

class Chess {
	friend class ChessAI;

	unsigned char m_board[_SIZE][_SIZE] = { 0 };
	bool m_turn = true;
	string m_boardString;
	string m_input;
	string m_msg = "\n";
	string m_errorMsg = "\n";
	int m_codeResponse;

	ChessAI m_ai;

	void clear() const;
	void setFrames();
	void setPieces();
	void show() const;
	void displayBoard() const;
	void showAskInput() const;
	bool isSame() const;
	bool isValid() const;
	bool isExit() const;
	void excute();
	void doTurn();

public:
	Chess(const string& start = "RNBQKBNRPPPPPPPP################################pppppppprnbqkbnr");
	Chess(const Chess&)=delete;
	Chess& operator=(const Chess&) = delete;
	string getInput();
	void setCodeResponse(int codeResponse);

	void createBoardFromString(const std::string& boardStr, Piece* board[8][8]);
	void calculateResponseCode();
	const string& getBoardString() const { return m_boardString; }
	bool getTurn() const { return m_turn; }

	std::string getBestMove(int depth);
	void printBestMoves(int count);

};

