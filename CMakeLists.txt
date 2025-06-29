cmake_minimum_required(VERSION 3.30)
project(homework01ex)

set(CMAKE_CXX_STANDARD 14)

add_executable(homework01ex main.cpp
        Board.h
        Bishop.h
        Piece.h
        King.h
        Knight.h
        Pawn.h
        Queen.h
        Rook.h
        Bishop.cpp
        King.cpp
        Knight.cpp
        Pawn.cpp
        Queen.cpp
        Rook.cpp
        Chess.cpp
        Chess.h
        Board.cpp)
