#include "Board.h"
#include "Knight.h"
#include "Rook.h"
#include "Bishop.h"
#include "Pawn.h"
#include "Queen.h"
#include "King.h"
#include "Piece.h"
<<<<<<< HEAD
=======
#include "InvalidSuicideException.h"
#include "InvalidMoveBaseException.h"
#include "InvalidPromotionException.h"
>>>>>>> 9c9a558 (Initial commit from new project chess-part2)
#include <iostream>

using namespace std;

// Constructor
Board::Board() {
    initializeBoard();
}

// Destructor
Board::~Board() {
     //Cleanup memory for each piece on the board
    for (int i = 0; i < BOARD_SIZE; ++i) {
        for (int j = 0; j < BOARD_SIZE; ++j) {
            delete board[i][j];
        }
    }
}
void Board::initializeBoard() {
    for (int i = 0; i < 8; ++i) {
        for (int j = 0; j < 8; ++j) {
            board[i][j] = nullptr;
        }
    }

    //White Team
    board[0][0] = new Rook(true);
    board[0][1] = new Knight(true);
    board[0][2] = new Bishop(true);
    board[0][3] = new Queen(true);
    board[0][4] = new King(true);
    board[0][5] = new Bishop(true);
    board[0][6] = new Knight(true);
    board[0][7] = new Rook(true);


    //Black Team
    board[7][0] = new Rook(false);
    board[7][1] = new Knight(false);
    board[7][2] = new Bishop(false);
    board[7][3] = new Queen(false);
    board[7][4] = new King(false);
    board[7][5] = new Bishop(false);
    board[7][6] = new Knight(false);
    board[7][7] = new Rook(false);

    for (int i = 0; i < 8; i++)
    {
        //White Pawn
        board[1][i] = new Pawn(true);
        //Black Pawn
        board[6][i] = new Pawn(false);
    }
}

<<<<<<< HEAD
int Board::movePiece(int from_x,int from_y,int to_x,int to_y)
{
    Piece* currentPiece = getPieceAt(from_x,from_y);
    if(currentPiece == nullptr)
        return 11;
    if(currentPiece->isWhite() != iswhiteturn)
        return 12;
    Piece* dest = getPieceAt(to_x,to_y);
    if(dest && currentPiece->isWhite() == dest->isWhite())
        return 13;
    if(currentPiece->isValidMove(from_x, from_y , to_x,to_y, *this))
    {
        setPieceAt(to_x,to_y,currentPiece);
        setPieceAt(from_x,from_y, nullptr);
        if(isKingInCheck(currentPiece->isWhite()))
        {
            setPieceAt(from_x, from_y, currentPiece);
            setPieceAt(to_x, to_y, dest);
            return 31;
        }
        else if (isKingInCheck(!iswhiteturn)) {
            iswhiteturn = !iswhiteturn;
            return 41;
        }
        iswhiteturn = !iswhiteturn;
        printBoard();
        return 42;
    }
    return 21;
}

=======
int Board::movePiece(int from_x, int from_y, int to_x, int to_y)
{
    Piece* currentPiece = getPieceAt(from_x, from_y);

    if (currentPiece == nullptr)
        throw InvalidMoveBaseException("No piece at source location."); // still returns 11 from outside

    if (currentPiece->isWhite() != iswhiteturn)
        throw InvalidMoveBaseException("This is not your turn."); // still returns 12 from outside

    Piece* dest = getPieceAt(to_x, to_y);

    if (dest && currentPiece->isWhite() == dest->isWhite())
        return 13;  // same-color collision (not throwing in this case)

    if (currentPiece->isValidMove(from_x, from_y, to_x, to_y, *this))
    {
        // Perform move temporarily
        setPieceAt(to_x, to_y, currentPiece);
        setPieceAt(from_x, from_y, nullptr);

        // Check if player is in check after the move
        if (isKingInCheck(currentPiece->isWhite()))
        {
            // Undo move
            setPieceAt(from_x, from_y, currentPiece);
            setPieceAt(to_x, to_y, dest);

            // Signal suicide move
            throw InvalidSuicideException("This move leaves your king in check.");
            return 31;  // still return for UI that doesn't use exceptions
        }

        // Promotion check
        if (currentPiece->getName() == "P" || currentPiece->getName() == "p")
        {
            bool isWhite = currentPiece->isWhite();
            if ((isWhite && to_x == 7) || (!isWhite && to_x == 0))
            {
                char choice;
                std::cout << "Pawn reached promotion! Choose Q/R/B/N: ";
                std::cin >> choice;
                choice = std::toupper(choice);

                Piece* newPiece = nullptr;
                switch (choice)
                {
                    case 'Q': newPiece = new Queen(isWhite); break;
                    case 'R': newPiece = new Rook(isWhite); break;
                    case 'B': newPiece = new Bishop(isWhite); break;
                    case 'N': newPiece = new Knight(isWhite); break;
                    default:
                        // Undo promotion, restore pawn
                        setPieceAt(from_x, from_y, currentPiece);
                        setPieceAt(to_x, to_y, dest);
                        throw InvalidPromotionException("Invalid promotion choice. Must be Q, R, B or N.");
                }

                delete currentPiece;
                setPieceAt(to_x, to_y, newPiece);
            }
        }

        // Finish move
        if (dest) delete dest;

        iswhiteturn = !iswhiteturn;
        printBoard();
        return 42; // successful move
    }

    return 21; // invalid move for this piece
}



>>>>>>> 9c9a558 (Initial commit from new project chess-part2)
bool Board::isKingInCheck(bool white) {
    // Find the position of the king
    Piece* kingSquare = nullptr;
    int kingLocationX , kingLocationY;
    string kingName = white ? "K" : "k";
    for (int i = 0; i < 8; ++i) {
        for (int j = 0; j < 8; ++j) {
            Piece* piece = board[i][j];
            if (piece && piece->isWhite() == white && piece->getName() == kingName) {
                kingSquare = board[i][j];
                kingLocationX = i;
                kingLocationY = j;
                break;
            }
        }
        if (kingSquare) break;
    }
    // Check if any opposing piece can attack the king
    for (int i = 0; i < 8; ++i) {
        for (int j = 0; j < 8; ++j) {
            Piece* piece = board[i][j];
            if (piece && piece->isWhite() != white) {
                if (piece->isValidMove(i, j, kingLocationX, kingLocationY, *this)) {
                    return true;
                }
            }
        }
    }
    return false;
}
//getters
Piece* Board::getPieceAt(int row, int col) const {
<<<<<<< HEAD
=======
    if (row < 0 || row >= 8 || col < 0 || col >= 8)
        return nullptr;
>>>>>>> 9c9a558 (Initial commit from new project chess-part2)
    return board[row][col];
}

//setters
void Board::setPieceAt(int row , int col , Piece* piece) {
    board[row][col] = piece;
}

void Board::printBoard() {
    using namespace std;
    cout << "   y: 0  1  2  3  4  5  6  7 " << endl << "x:" << endl;
<<<<<<< HEAD
    for (int i = 0; i < 8; i++)
    {
        cout << " " << i << "   ";
        for (int j = 0; j < 8; j++)
        {
            Piece* p = board[i][j];
            if(p!= nullptr)
                cout << " " <<p->getName()<<" ";
            else
                cout<<" "<<" "<<" ";
=======
    for (int i = 0; i < 8; i++) {
        cout << " " << i << "   ";
        for (int j = 0; j < 8; j++) {
            Piece *p = board[i][j];
            if (p != nullptr)
                cout << " " << p->getName() << " ";
            else
                cout << " " << " " << " ";
>>>>>>> 9c9a558 (Initial commit from new project chess-part2)
        }
        cout << endl;
    }
}

<<<<<<< HEAD
=======
Board* Board::createSimulatedCopy() const {
    Board* newBoard = new Board();
    newBoard->initializeBoard();
    for (int i = 0; i < 8; ++i) {
        for (int j = 0; j < 8; ++j) {
            Piece* piece = getPieceAt(i, j);
            if (piece) {
                std::string name = piece->getName();
                bool isWhite = piece->isWhite();
                if (name == "P" || name == "p") newBoard->setPieceAt(i, j, new Pawn(isWhite));
                else if (name == "R" || name == "r") newBoard->setPieceAt(i, j, new Rook(isWhite));
                else if (name == "N" || name == "n") newBoard->setPieceAt(i, j, new Knight(isWhite));
                else if (name == "B" || name == "b") newBoard->setPieceAt(i, j, new Bishop(isWhite));
                else if (name == "Q" || name == "q") newBoard->setPieceAt(i, j, new Queen(isWhite));
                else if (name == "K" || name == "k") newBoard->setPieceAt(i, j, new King(isWhite));
            }
        }
    }

    newBoard->iswhiteturn = this->getTurn();
    return newBoard;
}
>>>>>>> 9c9a558 (Initial commit from new project chess-part2)
