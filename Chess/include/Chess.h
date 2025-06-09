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

	int depth = 0;
	ChessAI m_ai;
	int winner = -1;

	void clear() const;
	void setFrames();
	void setPieces();
	void show() const;
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

	void displayBoard() const;

	bool isCheck(bool whiteTurnAfterMove, Piece* board[8][8]) const;
	bool isCheckmate(bool whiteKing, Piece* board[8][8]) const;
	int getWinner() { return winner; }

	void createBoardFromString(const std::string& boardStr, Piece* board[8][8]);
	void calculateResponseCode();
	const string& getBoardString() const { return m_boardString; }
	bool getTurn() const { return m_turn; }

	void setDepth(int depth) { this->depth = depth; }
	std::string getBestMove(int depth);
	void printBestMoves(int count);


	std::vector<std::string> getPiecesOfCurrentTurn();
	std::string getBestMoveForPiece(const std::string& piecePos);
	bool playMove(const std::string& move);

	enum ResponseCode {
		NO_PIECE = 11,
		WRONG_TURN = 12,
		SAME_COLOR = 13,
		ILLEGAL_MOVE = 21,
		CHECK = 41,
		VALID_MOVE = 42,
		CHECKMATE = 43
	};
};

