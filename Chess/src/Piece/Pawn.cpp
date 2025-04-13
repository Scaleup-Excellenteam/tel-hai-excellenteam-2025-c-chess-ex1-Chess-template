#include "Pawn.h"
#include "PieceFactory.h"
#include "indexStruct.h"
#include "Board.h"

bool Pawn::registerIt = PieceFactory::registerPiece("P", [](bool isWhite) -> std::unique_ptr<Piece> {
    return std::make_unique<Pawn>(isWhite);
    });

// Pawn movement:
// For simplicity, assume:
// - A pawn moves one step forward (direction depends on color).
// - It can move diagonally by one square for capturing.
bool Pawn::checkMoveRange(const indexPair& source, const indexPair& destination, const Board& board) const {

    int rowDiff = destination.row - source.row;
    int colDiff = destination.col - source.col;
    int forward = isWhite() ? 1 : -1;

    Piece* targetPiece = board.getPiece(destination);
    bool onStartRow = isWhite() ? (source.row == 1) : (source.row == 6);

    // Diagonal capture (must be enemy)
    if (rowDiff == forward && std::abs(colDiff) == 1)
        return targetPiece && targetPiece->isWhite() != isWhite();

    // One-square forward (must be empty)
    if (colDiff == 0 && rowDiff == forward)
        return !targetPiece;

    // Two-square forward (must be start row and path clear)
    if (colDiff == 0 && rowDiff == 2 * forward && onStartRow) {
        return board.isPathClear(source, destination) && !targetPiece;
    }

    return false;
}

