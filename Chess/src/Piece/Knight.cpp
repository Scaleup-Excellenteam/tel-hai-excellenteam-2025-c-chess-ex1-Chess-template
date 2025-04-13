#include "Knight.h"
#include "PieceFactory.h"
#include "indexStruct.h"
#include <cmath>
#include "Board.h"

bool Knight::registerIt = PieceFactory::registerPiece("N", [](bool isWhite) -> std::unique_ptr<Piece> {
    return std::make_unique<Knight>(isWhite);
    });

// Knight moves in an "L" shape: two squares in one direction and one square perpendicular.
bool Knight::checkMoveRange(const indexPair& source, const indexPair& destination, const Board& board) const
{
    int rowDiff = std::abs(source.row - destination.row);
    int colDiff = std::abs(source.col - destination.col);
    return (rowDiff == 2 && colDiff == 1) || (rowDiff == 1 && colDiff == 2);
}
