# Chess/CMakeLists.txt
cmake_minimum_required(VERSION 3.10)
project(ChessProject LANGUAGES CXX C)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Ensure the executable can find the PTHREAD library
find_package(Threads REQUIRED)

# Define a library for all common source files (GameEngine, Board, Pieces, AI, Utils)
# This promotes modularity and faster recompilation
add_library(ChessLib STATIC
    src/Board.cpp
    src/Pieces/Bishop.cpp
    src/Pieces/King.cpp
    src/Pieces/Knight.cpp
    src/Pieces/Pawn.cpp
    src/Pieces/Queen.cpp
    src/Pieces/Rook.cpp
    src/Pieces/Piece.cpp # <--- Ensure this is included
    src/Utils/ThreadPool.cpp
    src/AI/BestMoveFinder.cpp
    src/GameEngine.cpp
    src/Chess.cpp
)

# Set include directories for the library
target_include_directories(ChessLib PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/include
    ${CMAKE_CURRENT_SOURCE_DIR}/include/Pieces
    ${CMAKE_CURRENT_SOURCE_DIR}/include/Utils
    ${CMAKE_CURRENT_SOURCE_DIR}/include/AI
)

# Link the library to the Threads package
target_link_libraries(ChessLib PRIVATE Threads::Threads)

# Define the main executable
add_executable(Chess src/main.cpp)

# Link the executable to the library
target_link_libraries(Chess PRIVATE ChessLib)

# Optional: Set output directory for executables/libraries
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/lib")
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/lib")
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}") # Executable goes to build dir