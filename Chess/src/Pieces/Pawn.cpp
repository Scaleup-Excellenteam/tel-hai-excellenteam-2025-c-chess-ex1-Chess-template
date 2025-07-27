#include "Pieces/Pawn.h"
#include "Utils/Colors.h"
#include "Board.h" // For Board::getLastMove()
#include <cmath> // For std::abs


Pawn::Pawn(bool isWhite) : Piece(isWhite) {
    this->setSymbol(isWhite ? Colors::Pieces::WHITE_PAWN : Colors::Pieces::BLACK_PAWN);
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
    if (destRow1 >= 0 && destRow1 < 8 && b.getPiece(destRow1, destCol1) == nullptr) { // Must be empty
        moves.emplace_back(r, c, destRow1, destCol1);
    }

    // Double step forward
    if (r == home) {
        int destRow2 = r + 2 * dir;
        int destCol2 = c;
        if (destRow2 >= 0 && destRow2 < 8 && b.getPiece(destRow1, destCol1) == nullptr && b.getPiece(destRow2, destCol2) == nullptr) { // Path must be clear
            moves.emplace_back(r, c, destRow2, destCol2);
        }
    }

    // Diagonal captures (normal captures)
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

    // En-Passant logic
    // Check both potential en passant capture squares
    int enPassantCaptureRow = r + dir;
    int enPassantColOffset[] = {-1, 1}; // Check left and right diagonal for target

    for (int offset : enPassantColOffset) {
        int enPassantCaptureCol = c + offset;
        if (enPassantCaptureRow >= 0 && enPassantCaptureRow < 8 &&
            enPassantCaptureCol >= 0 && enPassantCaptureCol < 8) {
            if (isValidEnPassant(r, c, enPassantCaptureRow, enPassantCaptureCol, b)) {
                moves.emplace_back(r, c, enPassantCaptureRow, enPassantCaptureCol);
            }
        }
    }

    return moves;
}

// isValidEnPassant implementation
bool Pawn::isValidEnPassant(int srcR, int srcC, int destR, int destC, const Board& board) const {
    const int dir = getIsWhite() ? -1 : +1; // Direction pawn moves
    const int enPassantRank = getIsWhite() ? 3 : 4; // Rank pawn must be on to capture en passant (F-rank for white, D-rank for black)

    // Attacking pawn must be on its 5th rank (row 3 for white, row 4 for black)
    if (srcR != enPassantRank) {
        return false;
    }

    // Destination must be diagonally one square forward from attacker
    if (destR != srcR + dir || std::abs(destC - srcC) != 1) {
        return false;
    }

    // Destination square must be empty (captured pawn is NOT at destR,destC)
    if (board.getPiece(destR, destC) != nullptr) {
        return false;
    }

    // Opponent's pawn must be on the square directly beside the attacking pawn - in the same row
    // and must have just moved two squares from its starting position.
    int capturedPawnRow = srcR; // Same row as attacking pawn
    int capturedPawnCol = destC; // Same column as destination i.e., beside the attacking pawn

    const Piece* capturedPiece = board.getPiece(capturedPawnRow, capturedPawnCol);
    if (capturedPiece == nullptr || capturedPiece->getIsWhite() == getIsWhite()) {
        return false; // No piece, or it's your own piece
    }

    const Pawn* opponentPawn = dynamic_cast<const Pawn*>(capturedPiece);
    if (opponentPawn == nullptr) {
        return false; // Not a pawn
    }

    // Opponent's pawn must have just moved two squares in the previous turn.
    CMove lastMove = board.getLastMove();
    if (lastMove.destRow == capturedPawnRow && lastMove.destCol == capturedPawnCol && // Last move ended on the captured pawn's square
        lastMove.srcCol == capturedPawnCol && // Moved in the same column
        std::abs(lastMove.srcRow - lastMove.destRow) == 2 && // Moved two squares
        dynamic_cast<const Pawn*>(board.getPiece(lastMove.destRow, lastMove.destCol)) != nullptr && // The piece that moved was a pawn
        board.getPiece(lastMove.destRow, lastMove.destCol)->getIsWhite() != this->getIsWhite()) // And it was an opponent's pawn
    {
        return true;
    }

    return false;
}

bool Pawn::isValidMove(int sR,int sC,int dR,int dC,const Board& b) const
{
    const int dir  = getIsWhite() ? -1 : +1;   // white goes “up”  (row--)

    if (dC == sC) {
        if (dR == sR + dir && b.getPiece(dR,dC) == nullptr)               return true;
        const int home = getIsWhite() ?  6 :  1;   // g-rank / b-rank start
        if (sR == home && dR == sR + 2*dir &&
            b.getPiece(sR+dir,sC) == nullptr && b.getPiece(dR,dC)==nullptr) return true;
    }
    /* diagonal capture (normal or en passant) */
    if (std::abs(dC - sC) == 1 && dR == sR + dir) {
        const Piece* tgt = b.getPiece(dR,dC);
        if (tgt && tgt->getIsWhite() != this->getIsWhite())               return true;
        // En-Passant check
        if (isValidEnPassant(sR, sC, dR, dC, b)) {
            return true;
        }
    }
    return false;
}

bool Pawn::isValidCapture(int srcRow, int srcCol, int destRow, int destCol, const Board& board) const {
    const int dir = getIsWhite() ? -1 : +1; // White moves "up" (row--), Black moves "down" (row++)

    // Check for a normal diagonal capture move in the correct direction
    if (destRow == srcRow + dir && (destCol == srcCol - 1 || destCol == srcCol + 1)) {
        const Piece* destPiece = board.getPiece(destRow, destCol);
        if (destPiece && destPiece->getIsWhite() != this->getIsWhite()) {
            return true;
        }
    }
    // En-Passant capture check for `isInCheck`
    if (isValidEnPassant(srcRow, srcCol, destRow, destCol, board)) {
        return true;
    }
    return false;
}