#include "Bishop.h"
#include "PieceFactory.h"
#include "indexStruct.h"
#include <cstdlib>
#include "Board.h"

bool Bishop::registerIt = PieceFactory::registerPiece("B", [](bool isWhite) -> std::unique_ptr<Piece> {
    return std::make_unique<Bishop>(isWhite);
    });

// Bishop moves diagonally: the absolute row difference equals the absolute column difference (and not zero).
bool Bishop::checkMoveRange(const indexPair& source, const indexPair& destination, const Board& board) const
{
    int rowDiff = std::abs(source.row - destination.row);
    int colDiff = std::abs(source.col - destination.col);
    return (rowDiff == colDiff && rowDiff != 0) && board.isPathClear(source, destination);
}