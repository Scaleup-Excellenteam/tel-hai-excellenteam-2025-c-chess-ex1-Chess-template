/*
* File: Board.h
* Description: Header file for the Board class, which represents a chessboard.
* It contains methods for initializing the board, validating moves, and checking for check conditions.
*/

#pragma once
#include <array>
#include <memory>
#include "Piece.h"
#include <string>
#include "indexStruct.h"

constexpr int BOARD_SIZE = 8;

struct MoveIndices {
	indexPair source;
	indexPair destination;
};

class Board {
public:
	Board() = delete;
	Board(const std::string& players); //c-tor
	int validateAndPerformAction(const std::string& action);
	bool isPathClear(const indexPair& source, const indexPair& destination) const;
	Piece* getPiece(indexPair index) const;

private:
	std::array<std::array<std::unique_ptr<Piece>, BOARD_SIZE>, BOARD_SIZE> m_Board;
	void initializeKing(int row, int col);
	void initializeBoard(const std::string& players);
	MoveIndices parseAction(const std::string& action) const;

	std::unique_ptr<Piece> simulateMove(const indexPair& source, const indexPair& destination);
	void undoMove(const indexPair& source, const indexPair& destination, std::unique_ptr<Piece>& capturedPiece);
	void movePiece(indexPair source, indexPair destination);

	void setKingPosition(bool isWhite, const indexPair& position);
	indexPair findKingPosition(bool isWhite) const;
	bool isKingInCheck(bool isWhite) const;
	
	//Tracks current player turn
	bool m_isWhiteTurn;

	// The kings
	Piece* m_whiteKing;
	Piece* m_blackKing;
	indexPair m_whiteKingPos;
	indexPair m_blackKingPos;
};