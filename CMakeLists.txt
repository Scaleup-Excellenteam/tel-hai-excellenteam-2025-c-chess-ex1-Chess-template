cmake_minimum_required(VERSION 3.15)
project(Chess)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

include_directories(Chess/include)

file(GLOB_RECURSE SRC_FILES
        "Chess/src/*.cpp"
        "Chess/src/*.h"
        "Chess/include/*.h"
)

add_executable(ChessGame ${SRC_FILES})
