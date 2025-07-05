cmake_minimum_required(VERSION 3.10)
project(ChessProject LANGUAGES CXX C)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

find_package(Threads REQUIRED)

# Define a library pointing to sources inside the "Chess" directory
add_library(ChessLib STATIC
    Chess/src/Board.cpp
    Chess/src/Pieces/Bishop.cpp
    Chess/src/Pieces/King.cpp
    Chess/src/Pieces/Knight.cpp
    Chess/src/Pieces/Pawn.cpp
    Chess/src/Pieces/Queen.cpp
    Chess/src/Pieces/Rook.cpp
    Chess/src/Pieces/Piece.cpp
    Chess/src/Utils/ThreadPool.cpp
    Chess/src/AI/BestMoveFinder.cpp
    Chess/src/GameEngine.cpp
    Chess/src/Chess.cpp
)

# Set include directories pointing inside the "Chess" directory
target_include_directories(ChessLib PUBLIC
    Chess/include
    Chess/include/Pieces
    Chess/include/Utils
    Chess/include/AI
)

# Link the library to the Threads package
target_link_libraries(ChessLib PRIVATE Threads::Threads)

# Define the main executable
add_executable(Chess Chess/src/main.cpp)

# Link the executable to the library
target_link_libraries(Chess ChessLib)

# Define the test executable
add_executable(GameTests Chess/test/GameTests.cpp)

# Link the test executable to the library
target_link_libraries(GameTests ChessLib)

# Add include directories for the test executable
target_include_directories(GameTests PRIVATE
    Chess/include
    Chess/include/Pieces
    Chess/include/Utils
    Chess/include/AI
    Chess/test
)

# Optional: ensure tests are built with the main project
add_dependencies(Chess GameTests)

# Set output directories for executables and libraries
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/lib")
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/lib")
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}")