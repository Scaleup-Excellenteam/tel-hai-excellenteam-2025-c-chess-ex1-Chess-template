#include "Board.h"
#include "Pieces/Pawn.h" // For Pawn specific checks in generateLegalMoves
// Include all piece types so we can use their legalMoves methods
#include "Pieces/Bishop.h"
#include "Pieces/King.h"
#include "Pieces/Knight.h"
#include "Pieces/Queen.h"
#include "Pieces/Rook.h"
#include "Utils/Colors.h" // <--- ADDED THIS INCLUDE for Colors::Pieces

Board::Board() {
    grid.reserve(8); // optional but avoids reallocations
    for (int i = 0; i < 8; ++i) {
        grid.emplace_back(8); // each row has 8 null unique_ptrs
    }
}

// --- copy constructor ---
Board::Board(const Board& other) {
    grid.reserve(8);
    // create an 8×8 grid of empty slots
    for (int r = 0; r < 8; ++r)
        grid.emplace_back(8);

    // deep‐clone each piece via its virtual clone()
    for (int r = 0; r < 8; ++r) {
        for (int c = 0; c < 8; ++c) {
            Piece* p = other.getPiece(r, c);
            if (p)
                grid[r][c] = p->clone();
        }
    }
}

// --- copy assignment operator ---
Board& Board::operator=(const Board& rhs) {
    if (this != &rhs) {
        // reset ourselves
        grid.clear();
        grid.reserve(8);
        for (int r = 0; r < 8; ++r)
            grid.emplace_back(8);

        // deep‐clone each piece
        for (int r = 0; r < 8; ++r) {
            for (int c = 0; c < 8; ++c) {
                Piece* p = rhs.getPiece(r, c);
                if (p)
                    grid[r][c] = p->clone();
            }
        }
    }
    return *this;
}

// --- generate all legal moves for the given side (no self‐checks) ---
std::vector<CMove> Board::generateLegalMoves(bool whiteToMove) const {
    std::vector<CMove> legalMoves; // Renamed from 'moves' to 'legalMoves' for clarity

    for (int r = 0; r < 8; ++r) {
        for (int c = 0; c < 8; ++c) {
            Piece* p = getPiece(r, c);
            if (!p || p->getIsWhite() != whiteToMove)
                continue; // Skip empty squares or opponent's pieces

            // Get pseudo-legal moves for this specific piece
            // Each piece now has its own optimized legalMoves implementation
            std::vector<CMove> pseudoLegalMoves = p->legalMoves(r, c, *this);

            for (const auto& move : pseudoLegalMoves) {
                // Simulate the move on a temporary board
                Board temp_board = *this; // Deep copy
                temp_board.applyMove(move);

                // Check if the move leaves our own king in check
                if (!temp_board.inCheck(whiteToMove)) {
                    legalMoves.push_back(move);
                }
            }
        }
    }

    return legalMoves;
}

// --- apply a move (mutates this Board) ---
void Board::applyMove(CMove m) {
    // take ownership of the moving piece
    std::unique_ptr<Piece> moving = removePiece(m.srcRow, m.srcCol);
    // overwrite any captured piece and place the mover
    grid[m.destRow][m.destCol] = std::move(moving);
    // TODO: Handle Pawn Promotion, Castling, En Passant during applyMove in later phases.
}


// --- is the given side’s king in check? ---
bool Board::inCheck(bool whiteKing) const {
    // locate the king
    int kingR = -1, kingC = -1;
    // Use the actual symbols from Colors namespace for comparison
    const std::string& kingSymbol = whiteKing ? Colors::Pieces::WHITE_KING : Colors::Pieces::BLACK_KING;

    for (int r = 0; r < 8 && kingR < 0; ++r) {
        for (int c = 0; c < 8; ++c) {
            Piece* p = getPiece(r, c);
            if (p && p->getSymbol() == kingSymbol) { // Compare with string symbol
                kingR = r; kingC = c;
                break;
            }
        }
    }
    if (kingR < 0)
        return false; // no king found => not in check (shouldn't happen in a valid game)

    // see if any enemy piece attacks that square
    for (int r = 0; r < 8; ++r) {
        for (int c = 0; c < 8; ++c) {
            Piece* p = getPiece(r, c);
            if (p && p->getIsWhite() != whiteKing) { // If it's an opponent's piece
                // pawn captures are special for isValidMove checks
                if (Pawn* pawn = dynamic_cast<Pawn*>(p)) {
                    if (pawn->isValidCapture(r, c, kingR, kingC, *this))
                        return true;
                } else if (p->isValidMove(r, c, kingR, kingC, *this)) {
                    return true;
                }
            }
        }
    }
    return false;
}

Piece* Board::getPiece(int row, int col) const {
    // Returns a raw pointer to the piece at the specified cell
    return grid[row][col].get();
}

void Board::setPiece(int row, int col, std::unique_ptr<Piece> piece) {
    // This function takes ownership of the piece
    // and places it in the specified cell of the grid.
    grid[row][col] = std::move(piece);
}

std::unique_ptr<Piece> Board::removePiece(int row, int col) {
    std::unique_ptr<Piece> old = std::move(grid[row][col]);
    grid[row][col] = nullptr;
    return old;
}