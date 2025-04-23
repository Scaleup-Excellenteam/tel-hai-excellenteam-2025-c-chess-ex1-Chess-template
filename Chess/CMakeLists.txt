cmake_minimum_required(VERSION 3.15)

project(Chess)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

add_executable(Chess "")

add_subdirectory(include)
add_subdirectory(src)
