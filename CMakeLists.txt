cmake_minimum_required(VERSION 3.16)

project(ChessEngine LANGUAGES CXX)

# Use the C++17 standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Source files used to build the executable
add_executable(ChessEngine
    src/main.cpp
    src/Board.cpp
)

# Tell the compiler where to find header files
target_include_directories(ChessEngine PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/include
)