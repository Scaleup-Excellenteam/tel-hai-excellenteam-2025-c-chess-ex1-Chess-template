cmake_minimum_required (VERSION 3.15)

project (Chess)

# New CMake versions map C++20 to /std:c++20 instead of c++latest, so <format>
# and other unstable ABI features aren't supported. Use C++23 for such versions
if (MSVC AND CMAKE_VERSION VERSION_GREATER_EQUAL 3.21)
    set(CMAKE_CXX_STANDARD 23)
else ()
    set(CMAKE_CXX_STANDARD 20)
endif ()

set (CMAKE_CXX_STANDARD_REQUIRED ON)
set (CMAKE_CXX_EXTENSIONS OFF)

if (MSVC)
    add_compile_options (/W4 /permissive- /Zc:externConstexpr /Zc:inline)
endif ()

add_executable (Chess ""
        include/Common.h
        include/Piece.h
        include/Pieces.h
        include/Move.h
        include/Knight.h
        src/Move.cpp
        src/Pieces.cpp
        include/Pawn.h
        src/Piece.cpp
        src/Common.cpp
        include/Rook.h
        src/Rook.cpp
        include/Bishop.h
        include/King.h
        include/Queen.h
        src/Bishop.cpp
        src/Knight.cpp
        src/King.cpp
        src/Queen.cpp
        src/Pawn.cpp
        include/Constants.h
)


add_subdirectory (include)
add_subdirectory (src)



