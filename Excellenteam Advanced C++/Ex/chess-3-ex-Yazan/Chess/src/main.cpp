// Chess 
#include "Chess.h"
#include "RunChess.h"
#include <iostream>
#include <chrono>

using namespace std;


int main()
{

    // Print Start Message And Read The User Choose
    cout << "Choose A Game Mode:\n1. Human User vs Human User\n2.  Automatic Game" << endl;
    int gameMode;
    cin >> gameMode;

    if (gameMode == 2)
    {
        int threadNum, maxMoveNum;

        cout << "Please Enter The Threads Number (0 For No Threads, e.g. 2, 4, 8):" << endl;
        cin >> threadNum;

        cout << "Plaese Enter The Max Moves For The Automatic Game: " << endl;
        cin >> maxMoveNum;

        // Run The Game With Automatic Model.
        RunChess runTheChess;
        runTheChess.runTheAutoGame(threadNum, maxMoveNum);
    }

    // Otherwise Run Normal Game.
    else
    {
        string board = "RNBQKBNRPPPPPPPP################################pppppppprnbqkbnr";
        Chess a(board);
        int codeResponse = 0;
        string res = a.getInput();

        while (res != "exit")
        {
            cout << "code response >> ";
            cin >> codeResponse;

            a.setCodeResponse(codeResponse);
            res = a.getInput();
        }
        cout << endl << "Exiting " << endl;
    }
    return 0;
}