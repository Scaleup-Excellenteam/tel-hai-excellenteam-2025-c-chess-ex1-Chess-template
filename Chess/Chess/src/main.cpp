	// Chess 
#include "Chess.h"
#include "Board.h"

int main()
{
	//string board = "RNBQKBNRPPPPPPPP################################pppppppprnbqkbnr"; 
	string board = "##########K###############################R#############r#r#####";
	Chess a(board);
	int codeResponse = 0;
	bool isWhiteTurn = true;  // Track the turn locally
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
			// Run move validation logic
			Board boardLogic(board, isWhiteTurn);
			codeResponse = boardLogic.validateMove(res);

			// If move is legal (41 or 42), update board and switch turn
			if (codeResponse == 41 || codeResponse == 42)
			{
				// Actually move the piece on the board string
				auto [fromRow, fromCol] = boardLogic.parsePosition(res[0], res[1]);
				auto [toRow, toCol] = boardLogic.parsePosition(res[2], res[3]);

				int fromIndex = fromRow * 8 + fromCol;
				int toIndex = toRow * 8 + toCol;

				board[toIndex] = board[fromIndex];
				board[fromIndex] = '#';

				isWhiteTurn = !isWhiteTurn; // switch turn
			}

			/**/
			a.setCodeResponse(codeResponse);
			res = a.getInput(); 
		}

		cout << endl << "Exiting " << endl; 
		return 0;
	}
}