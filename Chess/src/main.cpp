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
	int numThreads = 0;
	bool isAuto = false;
	int autoChoice = -1;

	

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

	while (true) {
        std::cout << "Please select the number of threads to use (digit > 0): ";
        std::cin >> numThreads;

        if (numThreads <= 0) {
            std::cerr << "Invalid number of threads. Please enter a number greater than zero." << std::endl;
        } else {
            break;
        }
    }
	
    while (true) {
        std::cout << "Do you want the game to run in auto mode? (1 for Yes, 0 for No): ";
        std::cin >> autoChoice;

        if (autoChoice == 0 || autoChoice == 1) {
            isAuto = static_cast<bool>(autoChoice);
            break;
        } else {
            std::cerr << "Invalid choice. Please enter 1 for Yes or 0 for No." << std::endl;
        }
    }
	static GameBoard gameBoard;
	if(isAuto){
		gameBoard.runAutoGame(a, depth, numThreads);
	}
	else{

		MoveScore best = gameBoard.getBestMoveMultithreaded(depth, numThreads);
		int codeResponse = 0;
		string res = a.getInput(best.move,false);
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
			res = a.getInput(best.move,false); 
		}
	}

	cout << endl << "Exiting " << endl; 
	return 0;
}