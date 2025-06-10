cmake_minimum_required(VERSION 3.10)
project(ChessProject)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# ─── All .cpp sources for core game (including Chess.cpp!) ────────────────
set(SRC_FILES
    src/Chess.cpp
    src/main.cpp
    src/Board.cpp
    src/GameManager.cpp
    src/Utils/ThreadPool.cpp
    src/AI/BestMoveFinder.cpp
    src/Pieces/Bishop.cpp
    src/Pieces/King.cpp
    src/Pieces/Knight.cpp
    src/Pieces/Queen.cpp
    src/Pieces/Rook.cpp
    src/Pieces/Pawn.cpp
)

# ─── Build library target from those sources ──────────────────────────────
add_library(ChessLib STATIC ${SRC_FILES})
target_include_directories(ChessLib PUBLIC
    ${CMAKE_SOURCE_DIR}/include
)
find_package(Threads REQUIRED)
target_link_libraries(ChessLib PRIVATE Threads::Threads)

# ─── Build the actual executable (linking against ChessLib) ──────────────
add_executable(Chess
    src/main.cpp
)
target_link_libraries(Chess PRIVATE ChessLib)