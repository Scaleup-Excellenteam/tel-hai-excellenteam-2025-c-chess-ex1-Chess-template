#include "Pieces/Piece.h"
#include "Board.h" // Needed for Board access

// Default implementation of legalMoves for any piece can be overridden
std::vector<CMove> Piece::legalMoves(int r, int c, const Board& b) const
{
    std::vector<CMove> out;
    for (int dr = 0; dr < 8; ++dr)
        for (int dc = 0; dc < 8; ++dc)
        {
            if (dr == r && dc == c)       
                continue;

            if (isValidMove(r, c, dr, dc, b))
                out.emplace_back(r, c, dr, dc);
        }
    return out;
}