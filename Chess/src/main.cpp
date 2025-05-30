// Chess 
#include "../include/Chess.h"
#include "../include/GameBoard.h"
#include "../include/exceptions/InvalidDepthException.h" 
#include <iostream>


int main()
{
	string board = "RNBQKBNRPPPPPPPP################################pppppppprnbqkbnr"; 
//	string board = "##########K###############################R#############r#r#####";
	  // רק הסוס שיזוז

	Chess a(board);
	int depth = 0;
	int numThreads = 4;
	

	 while (true) {
		 std::cout << "Please select the depth of the recommendation moves (digit > 0): ";
		 std::cin >> depth;

        try {
            if (depth <= 0 ) {
                throw InvalidDepthException();
            }
            break;
        } catch (const InvalidDepthException& e) {
            std::cerr << e.what() << std::endl;
        }
    }

	static GameBoard gameBoard;
	MoveScore best = gameBoard.getBestMoveMultithreaded(depth, numThreads);

	int codeResponse = 0;
	string res = a.getInput(best.move);
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
			// gameBoard.displayBoard();

			
			codeResponse = gameBoard.handleMove(res);
			best = gameBoard.minimax(depth, gameBoard.getisWhiteTurn());
			
					
		}
		/**/

		a.setCodeResponse(codeResponse);
		res = a.getInput(best.move); 
	}

	cout << endl << "Exiting " << endl; 
	return 0;
}