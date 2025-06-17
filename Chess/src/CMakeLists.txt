add_executable(ChessGame
        main.cpp
        Chess.cpp
        Board.cpp
        Rook.cpp
        Piece.cpp
        Knight.cpp
        Bishop.cpp
        Pawn.cpp
        Queen.cpp
        King.cpp
        ThreadPool.cpp
        Model.cpp
)

target_include_directories(ChessGame PRIVATE ${CMAKE_CURRENT_LIST_DIR}/../include)

