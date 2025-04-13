#include "Rook.h"
#include "PieceFactory.h"
#include "indexStruct.h"
#include "Board.h"

bool Rook::registerIt = PieceFactory::registerPiece("R", [](bool isWhite) -> std::unique_ptr<Piece>
	{ return std::make_unique<Rook>(isWhite); });

// Rooks moves in straight lines, either horizontally or vertically.
bool Rook::checkMoveRange(const indexPair& source, const indexPair& destination, const Board& board) const
{
	return (source.row == destination.row || source.col == destination.col)
		&& (board.isPathClear(source, destination));
}
