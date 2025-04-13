#include "King.h"
#include "PieceFactory.h"
#include "indexStruct.h"
#include "Board.h"

bool King::registerIt = PieceFactory::registerPiece("K", [](bool isWhite) -> std::unique_ptr<Piece> { return std::make_unique<King>(isWhite); });

// Rooks moves in straight lines, either horizontally or vertically.
bool King::checkMoveRange(const indexPair& source, const indexPair& destination, const Board& board) const {
    int rowDiff = std::abs(source.row - destination.row);
    int colDiff = std::abs(source.col - destination.col);

    return (rowDiff <= 1 && colDiff <= 1) && (rowDiff + colDiff != 0);
}
