#pragma once

#include "Piece.h"
class Piece;
const int BOARD_SIZE = 8;
class Board {
private:
    bool iswhiteturn = true;
    Piece* board[BOARD_SIZE][BOARD_SIZE];
    Board();
    ~Board();

    // Private copy constructor and assignment operator to prevent copying
    Board(const Board&) = delete;
    Board& operator=(const Board&) = delete;
public:
    static Board& getInstance() {
        static Board instance;
        return instance;
    }

    void initializeBoard();

    //getters
    Piece* getPieceAt(int row, int col) const;

    //setters
    void setPieceAt(int row , int col ,Piece*);

    bool isKingInCheck(bool white);

    //moves
    int movePiece(int from_x,int from_y,int to_x,int to_y);

    void printBoard() ;

};


