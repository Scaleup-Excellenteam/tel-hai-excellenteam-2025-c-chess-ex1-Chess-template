#pragma once
#include "Piece.h"

struct indexPair;
class King : public Piece
{
public:
	King(bool isWhite) : Piece(isWhite) {};
	virtual bool checkMoveRange(const indexPair& source, const indexPair& destination, const Board& board) const override;
	virtual bool isKing() const override { return true; };

private:
	static bool registerIt;
};