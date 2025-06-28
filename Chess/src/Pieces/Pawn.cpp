#include "Pieces/Pawn.h"
#include "Utils/Colors.h" // Include for Unicode symbols
#include "Board.h"

Pawn::Pawn(bool isWhite) : Piece(isWhite) {
    this->setSymbol(isWhite ? Colors::Pieces::WHITE_PAWN : Colors::Pieces::BLACK_PAWN); // Set string symbol
    this->setIsAlive(true);
    this->setIsWhite(isWhite);
}

// Pawn's specific legalMoves implementation
std::vector<CMove> Pawn::legalMoves(int r, int c, const Board& b) const {
    std::vector<CMove> moves;
    const int dir = getIsWhite() ? -1 : +1; // white goes “up” (row--), black goes "down" (row++)
    const int home = getIsWhite() ? 6 : 1;  // starting row for double move

    // Single step forward
    int destRow1 = r + dir;
    int destCol1 = c;
    if (destRow1 >= 0 && destRow1 < 8 && isValidMove(r, c, destRow1, destCol1, b)) {
        moves.emplace_back(r, c, destRow1, destCol1);
    }

    // Double step forward
    if (r == home) {
        int destRow2 = r + 2 * dir;
        int destCol2 = c;
        if (destRow2 >= 0 && destRow2 < 8 && isValidMove(r, c, destRow2, destCol2, b)) {
            moves.emplace_back(r, c, destRow2, destCol2);
        }
    }

    // Diagonal captures (explicitly check both diagonal squares regardless of piece)
    int destRow_cap = r + dir;
    int destCol_cap1 = c - 1;
    int destCol_cap2 = c + 1;

    // Check left diagonal capture
    if (destRow_cap >= 0 && destRow_cap < 8 && destCol_cap1 >= 0 && destCol_cap1 < 8) {
        const Piece* target = b.getPiece(destRow_cap, destCol_cap1);
        if (target != nullptr && target->getIsWhite() != this->getIsWhite()) { // Must capture opponent piece
            moves.emplace_back(r, c, destRow_cap, destCol_cap1);
        }
    }

    // Check right diagonal capture
    if (destRow_cap >= 0 && destRow_cap < 8 && destCol_cap2 >= 0 && destCol_cap2 < 8) {
        const Piece* target = b.getPiece(destRow_cap, destCol_cap2);
        if (target != nullptr && target->getIsWhite() != this->getIsWhite()) { // Must capture opponent piece
            moves.emplace_back(r, c, destRow_cap, destCol_cap2);
        }
    }

    // TODO: Add En Passant logic here in a later phase to generate this special move.

    return moves;
}

bool Pawn::isValidMove(int sR,int sC,int dR,int dC,const Board& b) const
{
    const int dir  = getIsWhite() ? -1 : +1;   // white goes “up”  (row--)

    /* straight advance */
    if (dC == sC) {
        if (dR == sR + dir && b.getPiece(dR,dC) == nullptr)               return true;
        const int home = getIsWhite() ?  6 :  1;   // g-rank / b-rank start
        if (sR == home && dR == sR + 2*dir &&
            b.getPiece(sR+dir,sC) == nullptr && b.getPiece(dR,dC)==nullptr) return true;
    }
    /* diagonal capture */
    if (std::abs(dC - sC) == 1 && dR == sR + dir) {
        const Piece* tgt = b.getPiece(dR,dC);
        if (tgt && tgt->getIsWhite() != this->getIsWhite())               return true;
        // TODO: Add En Passant check here in a later phase.
    }
    return false;
}

bool Pawn::isValidCapture(int srcRow, int srcCol, int destRow, int destCol, const Board& board) const {
    const int dir = getIsWhite() ? -1 : +1; // White moves "up" (row--), Black moves "down" (row++)

    // Check for a diagonal capture move in the correct direction
    if (destRow == srcRow + dir && (destCol == srcCol - 1 || destCol == srcCol + 1)) {
        const Piece* destPiece = board.getPiece(destRow, destCol);
        // Ensure there is a piece to capture and it is an opponent's piece
        if (destPiece && destPiece->getIsWhite() != this->getIsWhite()) {
            return true;
        }
    }
    // TODO: Add En Passant capture check here in a later phase.
    return false;
}