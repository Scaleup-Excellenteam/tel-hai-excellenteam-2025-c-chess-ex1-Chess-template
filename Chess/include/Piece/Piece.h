#pragma once
#include <iostream>

/*
* File: Piece.h
* Description: Header file for the Piece class, which represents a chess piece.
*              It contains methods for checking move ranges and determining piece team.
*              every piece is derived from this class and registers itself in the PieceFactory class.
*/

struct indexPair;
class Board;
class Piece
{
public:
	Piece() = delete;
	Piece(bool isWhite) : m_isWhite(isWhite) {}; //c-tor
	virtual ~Piece() = default;

	//method to check if the move is valid for each individual piece
	virtual bool checkMoveRange(const indexPair& source, const indexPair& destination, const Board& board) const = 0;

	virtual bool isKing() const { return false; };
	const bool isWhite() const { return m_isWhite; };

private:
	bool m_isWhite;
};