#include "Queen.h"
#include "PieceFactory.h"
#include "indexStruct.h"
#include "Board.h"

bool Queen::registerIt = PieceFactory::registerPiece("Q", [](bool isWhite) -> std::unique_ptr<Piece> {
    return std::make_unique<Queen>(isWhite);
    });

// The Queen moves like both a Rook and a Bishop.
bool Queen::checkMoveRange(const indexPair& source, const indexPair& destination, const Board& board) const {
    int rowDiff = std::abs(source.row - destination.row);
    int colDiff = std::abs(source.col - destination.col);

    // Diagonal or straight line move
    if ((rowDiff == colDiff && rowDiff != 0) || source.row == destination.row || source.col == destination.col)
        return board.isPathClear(source, destination);

    return false;
}
