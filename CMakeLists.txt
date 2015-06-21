cmake_minimum_required(VERSION 3.2)
project(TidesTicTactics)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

set(SOURCE_FILES main.cpp)
add_executable(TidesTicTactics ${SOURCE_FILES} defs.h Board.cpp Board.h Miniboard.cpp Miniboard.h bitboards.cpp bitboards.h inits.h Engine.cpp Engine.h Transpositiontable.cpp Transpositiontable.h defs.cpp RatingTable.h IO.cpp IO.h)