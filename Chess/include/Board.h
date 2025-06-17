#pragma once

#include <vector>
#include <memory>
#include "Piece.h"

class Piece;
const int BOARD_SIZE = 8;

class Board {
private:
    bool iswhiteturn = true;
    Piece* board[BOARD_SIZE][BOARD_SIZE];

    // Private copy constructor and assignment operator to prevent copying
    Board(const Board&) = delete;
    Board& operator=(const Board&) = delete;

public:
    static Board& getInstance() {
        static Board instance;
        return instance;
    }

    void initializeBoard();

    // Getters
    [[nodiscard]] Piece* getPieceAt(int row, int col) const;
    [[nodiscard]] bool getTurn() const {
        return iswhiteturn;
    }

    // Setters
    void setPieceAt(int row , int col , Piece*);

    // Logic
    bool isKingInCheck(bool white) const;
    int movePiece(int from_x, int from_y, int to_x, int to_y);

    // Deep copy for simulation
    std::unique_ptr<Board> createSimulatedCopy() const;
    ~Board();

    Board();
};



