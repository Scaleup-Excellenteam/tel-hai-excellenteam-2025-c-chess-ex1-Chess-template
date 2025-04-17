// Chess 
#include "Chess.h"
#include "Board.h"
#include "GameUtils.h"

int main()
{
//	string board = "RNBQKBNRPPPPPPPP################################pppppppprnbqkbnr"; 
//	string board = "##########K###############################R#############r#r#####";
	string board = "RNBQKBNR################################################rnbqkbnr";

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
		{
			static Board internalBoard;
			static bool isBoardLoaded = false;
			static bool isWhiteTurn = true;

			// Input validation (mirror of Chess::isValid + isSame)
			if (res.length() != 4 ||
				res[0] < 'a' || res[0] > 'h' ||
				res[1] < '1' || res[1] > '8' ||
				res[2] < 'a' || res[2] > 'h' ||
				res[3] < '1' || res[3] > '8' ||
				(res[0] == res[2] && res[1] == res[3]))
			{
				codeResponse = 21;  // Invalid move
			}
			else
			{
				if (!isBoardLoaded) {
					loadBoardFromString(board, internalBoard);
					isBoardLoaded = true;
				}

				codeResponse = validateMove(res, internalBoard, isWhiteTurn);


			}
		}
		/**/

		a.setCodeResponse(codeResponse);
		res = a.getInput(); 
	}

	cout << endl << "Exiting " << endl; 
	return 0;
}