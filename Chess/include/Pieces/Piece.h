#pragma once
#include "Utils/CMove.h"
#include <memory>
#include <vector>
#include <string> // Added for string symbol

class Board; // Forward declaration

class Piece { // Abstract base class for chess pieces
private:
    bool isWhite;
    bool isAlive;
    std::string symbol; // Changed from char to std::string for Unicode

public:
    Piece(bool isWhite) : isWhite(isWhite), isAlive(true) {}
    virtual ~Piece() = default;

    // Check if this piece can move from src to dest
    virtual bool isValidMove(int srcRow, int srcCol, int destRow, int destCol, const Board& board) const = 0;
    virtual std::unique_ptr<Piece> clone() const = 0; // for cloning the piece

    // Generates pseudo-legal moves for this specific piece (does not check for self-check).
    // Default implementation iterates all squares, derived classes can optimize.
    virtual std::vector<CMove> legalMoves(int r, int c, const Board& b) const; // Now a true virtual method

    // getters and setters
    bool getIsAlive() const { return isAlive; }
    void setIsAlive(bool alive) { isAlive = alive; }
    void setSymbol(const std::string& sym) { symbol = sym; } // Changed to std::string
    const std::string& getSymbol() const { return symbol; } // Changed to std::string return type
    bool getIsWhite() const { return isWhite; }
    void setIsWhite(bool white) { isWhite = white; }
};