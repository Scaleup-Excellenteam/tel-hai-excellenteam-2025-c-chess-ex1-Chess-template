#pragma once

<<<<<<< HEAD
=======
#include <vector>
>>>>>>> 9c9a558 (Initial commit from new project chess-part2)
#include "Piece.h"
class Piece;
const int BOARD_SIZE = 8;
class Board {
private:
    bool iswhiteturn = true;
    Piece* board[BOARD_SIZE][BOARD_SIZE];
    Board();
<<<<<<< HEAD
    ~Board();
=======
>>>>>>> 9c9a558 (Initial commit from new project chess-part2)

    // Private copy constructor and assignment operator to prevent copying
    Board(const Board&) = delete;
    Board& operator=(const Board&) = delete;
public:
<<<<<<< HEAD
=======
    virtual ~Board();
>>>>>>> 9c9a558 (Initial commit from new project chess-part2)
    static Board& getInstance() {
        static Board instance;
        return instance;
    }

    void initializeBoard();

    //getters
<<<<<<< HEAD
    Piece* getPieceAt(int row, int col) const;
=======
    [[nodiscard]] Piece* getPieceAt(int row, int col) const;
>>>>>>> 9c9a558 (Initial commit from new project chess-part2)

    //setters
    void setPieceAt(int row , int col ,Piece*);

    bool isKingInCheck(bool white);

    //moves
    int movePiece(int from_x,int from_y,int to_x,int to_y);

    void printBoard() ;
<<<<<<< HEAD
=======
    [[nodiscard]] bool getTurn() const {
        return iswhiteturn;
    }
    Board* createSimulatedCopy() const;

>>>>>>> 9c9a558 (Initial commit from new project chess-part2)

};


