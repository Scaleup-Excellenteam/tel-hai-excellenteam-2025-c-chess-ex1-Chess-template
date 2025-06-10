#include "Pieces/Queen.h"
#include "Board.h"
#include "Pieces/Rook.h"
#include "Pieces/Bishop.h"
#include <cmath> // for abs()

Queen::Queen(bool isWhite) : Piece(isWhite) {
    char symbol = isWhite ? 'q' : 'Q'; // Assign symbol based on color
    this->setSymbol(symbol); // Set the symbol for the piece
    this->setIsAlive(true); // Set the alive status for the piece
    this->setIsWhite(isWhite); // Set the color of the piece
}

bool Queen::isValidMove(int srcRow,int srcCol,
                        int destRow,int destCol,
                        const Board& board) const
{
    // Get the actual color of this queen
    const bool is_white = this->getIsWhite();

    if (srcRow == destRow || srcCol == destCol) // rook-like
    {
        // Use the queen's actual color for the check
        return Rook(is_white).isValidMove(srcRow, srcCol, destRow, destCol, board);
    }

    if (std::abs(destRow - srcRow) == std::abs(destCol - srcCol)) // bishop-like
    {
        // Use the queen's actual color for the check
        return Bishop(is_white).isValidMove(srcRow, srcCol, destRow, destCol, board);
    }

    return false;
}