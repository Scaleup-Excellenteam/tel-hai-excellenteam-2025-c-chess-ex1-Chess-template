#include "RunChess.h"


// Check If The Player Piece Method.
inline bool RunChess::isThePlayerPiece(char piece, bool isWhitePlayer)
{
    return ((!isWhitePlayer && islower(piece)) ||
            (isWhitePlayer && isupper(piece)));
}


// Get All The Player Pieces Index's - Method.
vector<int> RunChess::getAllThePieces(const Chess &chess, bool whiteTurn)
{
    // Get The Current Board String, And Initialize A Vector For Indexes.
    const string &currentBoard = chess.getBoardString();
    vector<int> indexes;

    for (int i = 0; i < 64; i++)
    {
        // Check If Piece At Index i Is For The Current Player.
        if (isThePlayerPiece(currentBoard[i], whiteTurn))
        {
            indexes.push_back(i);
        }
    }
    return indexes;
}


// Calc The Best Single Player Move - Method.
void RunChess::calcBestMoveForPiece(const Chess &chess, MyPriorityQueue<Move> &movesQueue, int pieceIndex)
{
    LegalMove legalMove;
    Move move = legalMove.FindTheBestMove(chess, pieceIndex);

    // Check If Move Score Is Positive, And Add It To The Queue.
    if (move.getScore() > 0)
    {
        movesQueue.push(move);
    }
}


// Calculates Moves Simultaneously For All Tools - Method.
void RunChess::findMovesParallel(const Chess &chess, MyPriorityQueue<Move> &movesQueue, ThreadPool &pool, const vector<int> &pieceIndices)
{
    vector <future<void>> futures;

    // Create A Processes In Parallel With Each Tool.
    for (int index: pieceIndices)
    {
         pool.enqueue([this, &chess, &movesQueue, index]() {
           this->calcBestMoveForPiece(chess, movesQueue, index);
            });
    }
    // Makes Sure That All The Process Are Finish, And Wait For Them.
    for (auto &f: futures)
    {
        f.get();
    }
}


// Execute Move And Display Board - Method.
void RunChess::printBoardApplyMove(Chess &chess, const Move &move)
{
    // Get The Current Player Turn, And Print To The .
    bool whiteTurn = chess.getTurn();
    cout << "Turn " << (whiteTurn ? "White" : "Black")
         << "Plays: " << move.getFrom() << "->" << move.getTo() << endl;

    // Calc The Move And Set The Input, Code Response, And Print The Board
    string playerMove = move.getFrom() + move.getTo();
    chess.setInput(playerMove);
    chess.setCodeResponse(42);
    chess.displayBoard();
}


// Execute The Best Move - Method.
bool RunChess::runTheBestMove(Chess &chess, MyPriorityQueue<Move> &movesQueue)
{
    // Check If We Have Move To Execute, Execute It And Return True.
    if (!movesQueue.isEmpty())
    {
        const Move theBestMove = movesQueue.top();
        printBoardApplyMove(chess, theBestMove);
        return true;
    }
    // Otherwise Return False.
    else
    {
        return false;
    }
}


// Run Auto Game - Method.
void RunChess::runTheAutoGame(int nThreads, int maxMoves)
{
    // Init A ThreadPool Object, And The Initial Chess Board Game.
    string board = "RNBQKBNRPPPPPPPP################################pppppppprnbqkbnr";
    Chess a(board);
    ThreadPool threadPool(nThreads);

    // Save The Start Game Time.
    auto startTime = chrono::high_resolution_clock::now();

    for (int i = 0; i < maxMoves; i++)
    {
        // Check Any Player Is Turn, And Locates The Current Player's Pieces.
        bool isTheWhiteTurn = a.getTurn();
        vector<int> thePlayerPieces = getAllThePieces(a, isTheWhiteTurn);

        // Check If The Current Player Has Run Out Of Pieces.
        if (thePlayerPieces.empty())
        {
            cout << (isTheWhiteTurn ? "White" : "Black") << " No Pieces Left. " << endl;
            break;
        }

        // Init A Priority Queue And Calculates The Possible Moves In Parallel.
        MyPriorityQueue<Move> queue;
        findMovesParallel(a, queue, threadPool, thePlayerPieces);

        // Check If We Don't Have Legal Moves, And End The Game.
        if (!runTheBestMove(a, queue))
        {
            cout << "Don't Have Legal Moves For " << (isTheWhiteTurn ? "White" : "Black") << ". The Game Is Over!" << endl;
            break;
        }
    }

    // Calculate The Game Run Time And Print It.
    auto endTime = chrono::high_resolution_clock::now();
    chrono::duration<double> elapsed = endTime - startTime;
    cout << "The Chess Automatic Game Is Completed In " << elapsed.count() << " Seconds." << endl;
}