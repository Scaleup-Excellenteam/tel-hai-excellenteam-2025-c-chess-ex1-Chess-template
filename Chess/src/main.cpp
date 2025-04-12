// Chess 
#include "Chess.h"
#include "Board.h"
#include "Piece.h"
#include "Rook.h"
#include <cctype>
using namespace std;

int main()
{
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
