// Chess
#include "Chess.h"
#include "Pieces.h"
#include "Common.h"
#include "Move.h"
#include "Constants.h"

int main()
{
	string strBoard = "RNBQKBNRPPPPPPPP################################pppppppprnbqkbnr";
//	string strBoard = "##########K###############################R#############r#r#####";
    Chess a(strBoard);
	int codeResponse = 0;
    Pieces mainBoard(strBoard);
    COLOR turn = WHITE;
    mainBoard.updatePotenMoves(turn);

    string res = a.getInput();
	while (res != "exit")
	{
	// Convert the input string 'res' to a pair of Coordinates representing the source and target positions
        std::pair<Coordinate ,Coordinate> move = stringToCoordinate(res);
	// Flip coordinates to match the board orientation (e.g., user-facing vs internal board representation)	
        Coordinate source = flipChessCoord(move.first);
        Coordinate target = flipChessCoord(move.second);

	
	// Get shared pointers to the pieces at the source and target positions
        const std::shared_ptr<Piece>& sourcePiece = mainBoard.getPiece(source);
        const std::shared_ptr<Piece>& targetPiece = mainBoard.getPiece(target);

	// Error handling: Check various invalid move scenarios
        if(sourcePiece == nullptr)codeResponse = ERROR_NO_PIECE_AT_SOURCE;
	// The piece at the source does not belong to the current player
        else if(sourcePiece->getColor() != turn)codeResponse = ERROR_WRONG_COLOR_AT_SOURCE;
	// Target position has a friendly piece
        else if(targetPiece != nullptr && targetPiece->getColor() == turn)codeResponse = ERROR_TARGET_HAS_FRIENDLY;
	// The move is not valid according to the piece's movement rules
        else if(!sourcePiece->isValidMove(target , mainBoard))codeResponse = ERROR_ILLEGAL_MOVE;
	// The move would place the current player in check
        else if(sourcePiece->willBeInCheck(target , mainBoard))codeResponse = ERROR_MOVE_CAUSES_CHECK;
	// Move is valid, execute the move
        else {
            mainBoard.pieceMove(target , sourcePiece);// Move the piece to the target
            turn = colorNot(turn); // Switch the turn to the other player
            mainBoard.updatePotenMoves(turn); // Update possible moves for the new turn
	    // Check if the move has put the opponent in check
            mainBoard.isCheck(turn) ? codeResponse = STATUS_CHECK : codeResponse = STATUS_OK;
        }

        a.setCodeResponse(codeResponse);
		res = a.getInput();
	}

	cout << endl << "Exiting " << endl;
	return 0;
}










