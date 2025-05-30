#include "../include/GameBoard.h"
#include "../include/Rook.h"
#include "../include/King.h"
#include "../include/Bishop.h"
#include "../include/Queen.h"
#include "../include/Horse.h"
#include "../include/Pawn.h"
#include <string>
#include <iostream>
#include <cctype>

GameBoard::GameBoard() : isWhiteTurn(true) {
    for (int i = 0; i < 64; ++i) {
        int row = i / 8;
        int col = i % 8;
        if ((row == 0 && col == 0) || (row == 0 && col == 7)) {
            // Rooks for white
            board[row][col] = new Rook('R', true);
        } else if ((row == 7 && col == 0) || (row == 7 && col == 7)) {
            // Rooks for black
            board[row][col] = new Rook('r', false);
        } else if (row == 0 && col == 4) {
            // King for white
            board[row][col] = new King('K', true);
        } else if (row == 7 && col == 4) {
            // King for black
            board[row][col] = new King('k', false);
        } else if (row == 0 && col == 2 || row == 0 && col == 5) {
            // Bishop for white
            board[row][col] = new Bishop('B', true);
        } else if (row == 7 && col == 2 || row == 7 && col == 5) {
            // Bishop for black
            board[row][col] = new Bishop('b', false);
        } else if (row == 0 && col == 3) {
            // Queen for white
            board[row][col] = new Queen('Q', true);
        } else if (row == 7 && col == 3) {
            // Queen for black
            board[row][col] = new Queen('q', false);
        } else if (row == 0 && col == 1 || row == 0 && col == 6) {
            // Horse for white
            board[row][col] = new Horse('N', true);
        } else if (row == 7 && col == 1 || row == 7 && col == 6) {
            // Horse for black
            board[row][col] = new Horse('n', false);
        } else if (row == 1) {
             // Pawn for white
            board[row][col] = new Pawn('P', true);  
        } else if (row == 6) {
             // Pawn for black
            board[row][col] = new Pawn('p', false); 
        } else {
            board[row][col] = nullptr;
        }
    }
}

GameBoard::GameBoard(const GameBoard& other) {
    isWhiteTurn = other.isWhiteTurn;

    for (int i = 0; i < 8; ++i) {
        for (int j = 0; j < 8; ++j) {
            if (other.board[i][j] != nullptr) {
                board[i][j] = other.board[i][j]->clone();  // קריאה לפונקציה וירטואלית clone()
            } else {
                board[i][j] = nullptr;
            }
        }
    }
}


GameBoard::~GameBoard() {
    for (int i = 0; i < 8; ++i)
        for (int j = 0; j < 8; ++j)
            delete board[i][j];
}

Piece* GameBoard::getPiece(int row, int col) const {
    return board[row][col];
}

void GameBoard::movePiece(int fromRow, int fromCol, int toRow, int toCol) {

    board[toRow][toCol] = board[fromRow][fromCol]; // Move the piece
    board[fromRow][fromCol] = nullptr; // Clear the original position
}



int GameBoard::handleMove(const std::string& move) {
    if (move.length() != 4)
        return 21;  // Invalid move format

    // Validate move format
    if (!std::isalpha(move[0]) || !std::isdigit(move[1]) || 
        !std::isalpha(move[2]) || !std::isdigit(move[3]))
        return 21;  // Invalid characters in move

    // Convert the move coordinates
    int fromRow = move[0] - 'a';        // 'a' → 0
	int fromCol = move[1] - '1';        // '1' → 0
	int toRow   = move[2] - 'a';
	int toCol  = move[3] - '1';


    // Check if the move is within board boundaries
    if (fromCol < 0 || fromCol >= 8 || toCol < 0 || toCol >= 8 || 
        fromRow < 0 || fromRow >= 8 || toRow < 0 || toRow >= 8)
        return 21;  // Move out of bounds

    // Get the piece at the starting position
    Piece* piece = getPiece(fromRow, fromCol);
    if (!piece)
        return 11;  // No piece at the starting position

    if (piece && piece->getIsWhite() != isWhiteTurn) 
        return 12;  // Not your turn
       

    // Check if the move is valid for the piece
    if (!piece->isValidMove(fromRow, fromCol, toRow, toCol, getRawBoard()))
        return 21;  // Invalid move for the piece

    // Check if the destination is occupied by the same color piece
    Piece* destinationPiece = getPiece(toRow, toCol);
    if (destinationPiece && Piece::isSameColor(destinationPiece,piece))
        return 13;  // Can't capture your own piece

    // move for check if checkmate
    movePiece(fromRow, fromCol, toRow, toCol);

    if(isCheckmate(!getisWhiteTurn())){
        return 99;
    }
    // back to before 
    movePiece(toRow, toCol, fromRow, fromCol);

    if (isOwnKingInCheckAfterMove(fromRow, fromCol, toRow, toCol)) 
        return 31;  // Illegal move - exposes own king to check


    if (doesMoveCauseCheckToOpponent(fromRow, fromCol, toRow, toCol)){
        movePiece(fromRow, fromCol, toRow, toCol);
        switchTurn();
        return 41;
    }
       

    // Perform the move
    movePiece(fromRow, fromCol, toRow, toCol);

    switchTurn();


   
    return 42;  // Move successful
}


bool GameBoard::isOwnKingInCheckAfterMove(int fromRow, int fromCol, int toRow, int toCol) {
    Piece* movingPiece = board[fromRow][fromCol];
    if (!movingPiece) return false;

    Piece* captured = board[toRow][toCol];


    board[toRow][toCol] = movingPiece;
    board[fromRow][fromCol] = nullptr;


    int kingRow = -1, kingCol = -1;
    char kingSymbol = movingPiece->getIsWhite() ? 'K' : 'k';

    if (movingPiece->getSymbol() == kingSymbol) {
        kingRow = toRow;
        kingCol = toCol;
    } else {
        for (int row = 0; row < 8; ++row) {
            for (int col = 0; col < 8; ++col) {
                if (board[row][col] != nullptr &&
                    board[row][col]->getSymbol() == kingSymbol) {
                    kingRow = row;
                    kingCol = col;
                }
            }
        }
    }

    if (kingRow == -1 || kingCol == -1) {
        // std::cerr << "[ERROR] King not found on board after move!\n";
        return false;  
    }

    bool inCheck = false;
    for (int row = 0; row < 8 && !inCheck; ++row) {
        for (int col = 0; col < 8 && !inCheck; ++col) {
            Piece* attacker = board[row][col];
            if (attacker != nullptr && attacker->getIsWhite() != movingPiece->getIsWhite()) {
                if (attacker->isValidMove(row, col, kingRow, kingCol, board)) {
                    inCheck = true;
                }
            }
        }
    }


    board[fromRow][fromCol] = movingPiece;
    board[toRow][toCol] = captured;

    return inCheck;
}


bool GameBoard::doesMoveCauseCheckToOpponent(int fromX, int fromY, int toX, int toY) {
    Piece* movingPiece = board[fromX][fromY];
    if (!movingPiece) return false;

    Piece* captured = board[toX][toY];


    board[toX][toY] = movingPiece;
    board[fromX][fromY] = nullptr;

    bool opponentInCheck = false;

    int opponentKingX = -1, opponentKingY = -1;
    char opponentKingSymbol = movingPiece->getIsWhite() ? 'k' : 'K';

    for (int x = 0; x < 8; ++x)
        for (int y = 0; y < 8; ++y)
            if (board[x][y] != nullptr && board[x][y]->getSymbol() == opponentKingSymbol) {
                opponentKingX = x;
                opponentKingY = y;
            }


    for (int x = 0; x < 8; ++x)
        for (int y = 0; y < 8; ++y)
            if (board[x][y] != nullptr && board[x][y]->getIsWhite() == movingPiece->getIsWhite())
                if (board[x][y]->isValidMove(x, y, opponentKingX, opponentKingY, board))
                    opponentInCheck = true;


    board[fromX][fromY] = movingPiece;
    board[toX][toY] = captured;

    return opponentInCheck;
}


bool GameBoard::isCheckmate(bool isWhite) {
    int kingRow = -1, kingCol = -1;
    char kingSymbol = isWhite ? 'K' : 'k';

    //find the king location
    for (int row = 0; row < 8; ++row)
        for (int col = 0; col < 8; ++col)
            if (board[row][col] != nullptr && board[row][col]->getSymbol() == kingSymbol) {
                kingRow = row;
                kingCol = col;
            }

    // check if the king under threat
    bool inCheck = false;
    for (int row = 0; row < 8 && !inCheck; ++row)
        for (int col = 0; col < 8 && !inCheck; ++col)
            if (board[row][col] != nullptr && board[row][col]->getIsWhite() != isWhite)
                if (board[row][col]->isValidMove(row, col, kingRow, kingCol, board))
                    inCheck = true;

    if (!inCheck) return false;

    // try to avoid check
    for (int fromRow = 0; fromRow < 8; ++fromRow)
        for (int fromCol = 0; fromCol < 8; ++fromCol)
            if (board[fromRow][fromCol] != nullptr && board[fromRow][fromCol]->getIsWhite() == isWhite)
                for (int toRow = 0; toRow < 8; ++toRow)
                    for (int toCol = 0; toCol < 8; ++toCol) {
                        Piece* moving = board[fromRow][fromCol];
                        Piece* captured = board[toRow][toCol];

                        if (captured && Piece::isSameColor(captured, moving))
                            continue;

                        if (!moving->isValidMove(fromRow, fromCol, toRow, toCol, board))
                            continue;

                        //move temporary
                        board[toRow][toCol] = moving;
                        board[fromRow][fromCol] = nullptr;

                        // find king location
                        int newKingRow = kingRow, newKingCol = kingCol;
                        if (moving->getSymbol() == kingSymbol)
                            newKingRow = toRow, newKingCol = toCol;

                        // king steel under threat?
                        bool stillInCheck = false;
                        for (int row = 0; row < 8 && !stillInCheck; ++row)
                            for (int col = 0; col < 8 && !stillInCheck; ++col)
                                if (board[row][col] != nullptr &&
                                    board[row][col]->getIsWhite() != isWhite)
                                    if (board[row][col]->isValidMove(row, col, newKingRow, newKingCol, board))
                                        stillInCheck = true;

                        // restore board
                        board[fromRow][fromCol] = moving;
                        board[toRow][toCol] = captured;

                        // no checkmete
                        if (!stillInCheck)
                            return false;
                    }

    return true; // checkmate
}


int GameBoard::getPieceValue(char symbol) {
    symbol = tolower(symbol);
    int value = 0;
    switch (symbol) {
        case 'p': value = 10; break;    // Pawn
        case 'n': value = 30; break;    // Knight
        case 'b': value = 30; break;    // Bishop
        case 'r': value = 50; break;    // Rook
        case 'q': value = 100; break;   // Queen
        case 'k': value = 1000; break; // King
        default: break;
    }
    return value;
}

int GameBoard::weakPeaceThreat(bool currentPlayerIsWhite) {
    int penalty = 0;
    for (int fromX = 0; fromX < 8; ++fromX) {
        for (int fromY = 0; fromY < 8; ++fromY) {
            Piece* piece = this->board[fromX][fromY]; 
            if (piece && piece->getIsWhite() == currentPlayerIsWhite) {
                int myValue = getPieceValue(piece->getSymbol());
                for (int toX = 0; toX < 8; ++toX) {
                    for (int toY = 0; toY < 8; ++toY) {
                        Piece* attacker = this->board[toX][toY];  
                        if (attacker && attacker->getIsWhite() != currentPlayerIsWhite &&
                            attacker->isValidMove(toX, toY, fromX, fromY, this->board)) {
                            int attackerValue = getPieceValue(attacker->getSymbol());
                            if (attackerValue < myValue) {
                                penalty += (myValue - attackerValue);
                            }
                        }
                    }
                }
            }
        }
    }
    return -penalty;
}



int GameBoard::threatStrongerPiece(bool isWhiteTurn) {
    int score = 0;

    for (int fromX = 0; fromX < 8; ++fromX) {
        for (int fromY = 0; fromY < 8; ++fromY) {
            Piece* attacker = this->board[fromX][fromY];  
            if (attacker && attacker->getIsWhite() == isWhiteTurn) {
                int attackerValue = getPieceValue(attacker->getSymbol());

                for (int toX = 0; toX < 8; ++toX) {
                    for (int toY = 0; toY < 8; ++toY) {
                        Piece* target = this->board[toX][toY]; 
                        if (target && target->getIsWhite() != isWhiteTurn &&
                            attacker->isValidMove(fromX, fromY, toX, toY, this->board)) {
                            int targetValue = getPieceValue(target->getSymbol());
                            if (targetValue > attackerValue) {
                                score += (targetValue - attackerValue);
                            }
                        }
                    }
                }
            }
        }
    }
    return score;
}

int GameBoard::eatOponentPiece(int fromX, int fromY, int toX, int toY) {
    Piece* attacker = this->board[fromX][fromY]; 
    Piece* target = this->board[toX][toY]; 

    if (!attacker || !target) return 0;
    if (attacker->getIsWhite() == target->getIsWhite()) return 0;
    if (!attacker->isValidMove(fromX, fromY, toX, toY, this->board)) return 0;

    return getPieceValue(target->getSymbol());
}

int GameBoard::evaluateBoard(bool currentPlayerIsWhite) {
    int score = 0;
    for (int i = 0; i < 8; ++i) {
        for (int j = 0; j < 8; ++j) {
            Piece* piece = this->board[i][j]; 
            if (piece != nullptr) {
                int value = getPieceValue(piece->getSymbol());
                if (piece->getIsWhite() == currentPlayerIsWhite) {
                    score += value;
                } else {
                    score -= value;
                }
            }
        }
    }

    for (int fromX = 0; fromX < 8; ++fromX) {
        for (int fromY = 0; fromY < 8; ++fromY) {
            Piece* attacker = this->board[fromX][fromY]; 
            if (attacker && attacker->getIsWhite() == currentPlayerIsWhite) {
                for (int toX = 0; toX < 8; ++toX) {
                    for (int toY = 0; toY < 8; ++toY) {
                        int eatScore = eatOponentPiece(fromX, fromY, toX, toY); 
                        score += eatScore ;
                    }
                }
            }
            else{
                 for (int toX = 0; toX < 8; ++toX) {
                    for (int toY = 0; toY < 8; ++toY) {
                        int eatScore = eatOponentPiece(fromX, fromY, toX, toY); 
                        score -= eatScore ;
                    }
                }
            }
        }
    }

    score += weakPeaceThreat(currentPlayerIsWhite);
    score += threatStrongerPiece(currentPlayerIsWhite);
    return score;
}


MoveScore GameBoard::minimax(int depth, bool isMaximizingPlayer) {
    if (depth == 0) {
        return { Move{-1, -1, -1, -1}, evaluateBoard(isMaximizingPlayer) };
    }

    PriorityQueue pq;

    for (int fromRow = 0; fromRow < 8; ++fromRow) {
        for (int fromCol = 0; fromCol < 8; ++fromCol) {
            Piece* piece = this->board[fromRow][fromCol]; 
            if (piece && piece->getIsWhite() == isMaximizingPlayer) {
                for (int toRow = 0; toRow < 8; ++toRow) {
                    for (int toCol = 0; toCol < 8; ++toCol) {
                        if (piece->isValidMove(fromRow, fromCol, toRow, toCol, this->board)) { 
                            Piece* captured = this->board[toRow][toCol]; 

                            if (captured && Piece::isSameColor(captured, piece)) {
                                continue;
                            }
                            if(isOwnKingInCheckAfterMove(fromRow,fromCol,toRow,toCol)){
                                continue;
                            }

                            movePiece(fromRow, fromCol, toRow, toCol);
                            MoveScore result = minimax(depth - 1, !isMaximizingPlayer);
                            undoMove(fromRow, fromCol, toRow, toCol, piece, captured);

                            Move m = {fromRow, fromCol, toRow, toCol};
                            pq.push(m, result.score);
                        }
                    }
                }
            }
        }
    }

    if (!pq.isEmpty()) {
        return pq.pull(); 
    }

    return { Move{-1, -1, -1, -1}, isMaximizingPlayer ? -10000 : 10000 };
}


void GameBoard::undoMove(int fromRow, int fromCol, int toRow, int toCol, Piece* movedPiece, Piece* capturedPiece) {
    board[fromRow][fromCol] = movedPiece;
    board[toRow][toCol] = capturedPiece;
}

std::vector<std::pair<int, int>> GameBoard::getPlayerPieces(bool isWhite) const {
    std::vector<std::pair<int, int>> positions;
    for (int row = 0; row < 8; ++row) {
        for (int col = 0; col < 8; ++col) {
            Piece* piece = board[row][col];
            if (piece != nullptr && piece->getIsWhite() == isWhite) {
                positions.emplace_back(row, col);
            }
        }
    }
    return positions;
}

MoveScore GameBoard::getBestMoveMultithreaded(int depth, int numThreads) {
    std::vector<std::pair<int, int>> pieces = getPlayerPieces(isWhiteTurn);
    std::mutex resultsMutex;
    std::vector<MoveScore> allMoves;

    std::atomic<int> tasksRemaining = pieces.size();
    std::condition_variable allDone;
    std::mutex cvMutex;

    ThreadPool pool(numThreads); 

    for (const auto& [row, col] : pieces) {
        pool.enqueue([&, row, col]() {
            GameBoard localBoard = *this;
            Piece* piece = localBoard.getPiece(row, col);
            if (!piece) {
                if (--tasksRemaining == 0) {
                    std::lock_guard<std::mutex> lock(cvMutex);
                    allDone.notify_one();
                }
                return;
            }

            std::vector<std::pair<int, int>> moves = piece->getLegalMoves(row, col, localBoard.board);
            MoveScore bestMove = { { -1, -1, -1, -1 }, std::numeric_limits<int>::min() };

            for (const auto& [toRow, toCol] : moves) {
                if (localBoard.isOwnKingInCheckAfterMove(row, col, toRow, toCol)) {
                    continue;
                }

                Piece* captured = localBoard.getPiece(toRow, toCol);
                localBoard.movePiece(row, col, toRow, toCol);

                int score = localBoard.minimax(depth - 1, !isWhiteTurn).score;

                if (score > bestMove.score) {
                    bestMove = { {row, col, toRow, toCol}, score };
                }

                localBoard.undoMove(row, col, toRow, toCol, piece, captured);
            }

            {
                std::lock_guard<std::mutex> lock(resultsMutex);
                if (bestMove.move.fromRow != -1) {
                    allMoves.push_back(bestMove);
                }
            }

            if (--tasksRemaining == 0) {
                std::lock_guard<std::mutex> lock(cvMutex);
                allDone.notify_one();
            }
        });
    }

    {
        std::unique_lock<std::mutex> lock(cvMutex);
        allDone.wait(lock, [&]() { return tasksRemaining == 0; });
    }

    if (allMoves.empty()) {
        return { { -1, -1, -1, -1 }, isWhiteTurn ? -10000 : 10000 };
    }

    MoveScore bestOverall = allMoves[0];
    for (const auto& move : allMoves) {
        if (move.score > bestOverall.score) {
            bestOverall = move;
        }
    }

    return bestOverall;
}

void GameBoard::runAutoGame(Chess& chess, int depth, int numThreads) {
    using namespace std::chrono;

    std::unordered_map<std::string, int> positionCount; 

    auto start = high_resolution_clock::now();

    MoveScore best = getBestMoveMultithreaded(depth, numThreads);
    std::string res = chess.getInput(best.move, true);

    for ( int i = 0 ; i < 8 && res != "exit" ; i++) {
        int codeResponse = handleMove(res);
        chess.setCodeResponse(codeResponse);

      
        std::string positionHash = getCurrentPositionHash(); 

        positionCount[positionHash]++;
        if (positionCount[positionHash] >= 3) {
            std::cout << "Draw detected due to three repetition." << std::endl;
            break;
        }

        best = getBestMoveMultithreaded(depth, numThreads);
        res = chess.getInput(best.move, true);
    }

    auto end = high_resolution_clock::now();
    auto duration = duration_cast<milliseconds>(end - start);
    std::cout << "Auto game took " << duration.count() << " milliseconds." << std::endl;
}


std::string GameBoard::getCurrentPositionHash() {
    std::string hash;

    for (int row = 0; row < 8; row++) {
        int emptyCount = 0;
        for (int col = 0; col < 8; col++) {
            Piece* piece = board[row][col];
            if (piece == nullptr) {
                emptyCount++;
            } else {
                
                if (emptyCount > 0) {
                    hash += std::to_string(emptyCount);
                    emptyCount = 0;
                }
               
                hash += piece->getSymbol();  
            }
        }
        if (emptyCount > 0) {
            hash += std::to_string(emptyCount);
        }
        if (row < 7) {
            hash += '/';
        }
    }

    hash += (isWhiteTurn) ? " w" : " b";

    return hash;
}

