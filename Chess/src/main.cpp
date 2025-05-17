<<<<<<< HEAD
// Chess 
#include "Chess.h"
#include "Board.h"
#include "Piece.h"
#include "Rook.h"
#include <cctype>
=======
// Chess
#include "Chess.h"
#include "Board.h"
#include "Model.h"
#include "Move.h"
#include <iostream>
#include <algorithm>
#include "InvalidMoveBaseException.h"
#include "InvalidSuicideException.h"
#include "InvalidPromotionException.h"


>>>>>>> 9c9a558 (Initial commit from new project chess-part2)
using namespace std;

int main()
{
<<<<<<< HEAD
	string board = "RNBQKBNRPPPPPPPP################################pppppppprnbqkbnr"; 
//	string board = "##########K###############################R#############r#r#####";
	Chess a(board);
	int codeResponse = 0;
	string res = a.getInput();
	while (res != "exit")
	{
		/* 
		codeResponse value : 
		Illegal movements : 
		11 - there is not piece at the source  
		12 - the piece in the source is piece of your opponent
		13 - there one of your pieces at the destination 
		21 - illegal movement of that piece 
		31 - this movement will cause you checkmate

		legal movements : 
		41 - the last movement was legal and cause check 
		42 - the last movement was legal, next turn 
		*/

		/**/ 
		{ // put your code here instead that code

                  Board& selfMadeBoard = Board::getInstance();

                  int from_x = res[0] - 'a';
                  int from_y = res[1] - '0' - 1;
                  int to_x = res[2] - 'a';
                  int to_y = res[3] - '0' - 1;

                  codeResponse =  selfMadeBoard.movePiece(from_x , from_y, to_x , to_y);


		}
		a.setCodeResponse(codeResponse);
		res = a.getInput();
	
		}/**/


	cout << endl << "Exiting " << endl; 
	return 0;
}
=======
    // Initial board setup (standard position in 1D string)
    string board = "RNBQKBNRPPPPPPPP################################pppppppprnbqkbnr";
    Chess a(board); // Chess UI/engine wrapper
    int codeResponse = 0;

    // Get board instance and whose turn it is
    Board& selfMadeBoard = Board::getInstance();
    bool isWhiteTurn = selfMadeBoard.getTurn();

    // Show move suggestions before the first input
    auto bestMoves = Model::suggestMovesDepth2(selfMadeBoard, isWhiteTurn);
    cout << "Suggested moves for " << (isWhiteTurn ? "White" : "Black") << ":\n";
    for (int i = 0; i < std::min(3, (int)bestMoves.size()); ++i) {
        cout << "Recommended step: ";
        cout << "  " << moveToNotation(bestMoves[i]) + "\n";
    }

    string res = a.getInput(); // First input from user

    while (res != "exit")
    {
        int from_x = res[0] - 'a';
        int from_y = res[1] - '0' - 1;
        int to_x = res[2] - 'a';
        int to_y = res[3] - '0' - 1;

        try {
            codeResponse = selfMadeBoard.movePiece(from_x, from_y, to_x, to_y);
        } catch (const InvalidMoveBaseException& e) {
            std::cerr << "Invalid move: " << e.what() << std::endl;
            codeResponse = 12;
        } catch (const InvalidSuicideException& e) {
            std::cerr << "Suicide move: " << e.what() << std::endl;
            codeResponse = 31;
        } catch (const InvalidPromotionException& e) {
            std::cerr << "Invalid promotion: " << e.what() << std::endl;
            codeResponse = 32;
        }

        // Update the UI or log with result code
        a.setCodeResponse(codeResponse);

        //  Show move suggestions for the next turn
        isWhiteTurn = selfMadeBoard.getTurn();
        bestMoves = Model::suggestMovesDepth2(selfMadeBoard, isWhiteTurn);
        std::cout << "Suggested moves for " << (isWhiteTurn ? "White" : "Black") << ":\n";
        for (int i = 0; i < std::min(3, (int)bestMoves.size()); ++i) {
            std::cout << "  " << moveToNotation(bestMoves[i]) << " (Score: " << bestMoves[i].score << ")\n";
        }

        // Read next input
        res = a.getInput();
    }

    cout << endl << "Exiting " << endl;
    return 0;
}

>>>>>>> 9c9a558 (Initial commit from new project chess-part2)
