#include "LegalMove.h"


// If Legal Piece - Method.
bool LegalMove::isValidPiece(char piece)
{
    return isupper(piece);
}


// Calculate The Row Number Method.
inline int LegalMove::calcFromRow(int i)
{
    return i / 8;
}


// Calculate The Column Number Method.
inline int LegalMove::calcFromCol(int i)
{
    return i % 8;
}


// From Index To Notation - Method.
string LegalMove::indexToChessSquare(int index)
{
    int row = calcFromRow(index), column = calcFromCol(index);

    // Calc The File Letter, And The Rank Number.
    char file = 'a' + column, rank = '1' + row;

    // Build The Square String In Standard Chess Notation.
    string squareCoordinates;
    squareCoordinates += file;
    squareCoordinates += rank;

    return squareCoordinates;
}


// Try To Go Forward - Method.
bool LegalMove::findForwardMove(const string &boardStr, int i, string &from, string &to)
{
    int toRow = calcFromRow(i) + 1, toColumn = calcFromCol(i);

    // Calculate The Target Index By Moving One Row Forward.
    if (toRow < 8)
    {
        int moveToIndex = toRow * 8 + toColumn;

        // Check If The Target Index Is Vacant.
        if (boardStr[moveToIndex] == '#')
        {
            // Convert The Index's To Standard Chess Chars.
            from = indexToChessSquare(i);
            to = indexToChessSquare(moveToIndex);
            return true;
        }
    }
    return false;
}


// Find Best Move - Method.
Move LegalMove::FindTheBestMove(const Chess &chess, int pieceIndex)
{
    // Get The Current Board As String, And Init To Strings To Save The Final From And To Score.
    const string theBoard = chess.getBoardString();
    int theBigScore = -99999;
    string bestScoreFrom, bestScoreTo;


    // Get The Piece At The Current Index.
    char piece = theBoard[pieceIndex];
    if (isValidPiece(piece))
    {
        string from, to;
        // Try To Generate Forward Move For The Piece.
        if (findForwardMove(theBoard, pieceIndex, from, to))
        {
            // Found The Valid Move, And Update The Best Move.
            bestScoreFrom = from;
            bestScoreTo = to;
            theBigScore = 100;

            // Return The Valid Move We Had Found.
            return Move(theBigScore, bestScoreFrom, bestScoreTo);
        }
    }
    // If We Don't Find A Valid Move, Create A Feigned Move And Return It.
    return Move(0, "a1", "a1");
}
