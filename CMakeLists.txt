cmake_minimum_required(VERSION 3.22)
project(Fairy_Tail)

set(CMAKE_CXX_STANDARD 17)

add_executable(Fairy_Tail main.cpp
        fairyTail/fairy_tail.cpp fairyTail/fairy_tail.hpp
        maze/fairyMaze/fairyMaze.cpp maze/fairyMaze/fairyMaze.h
        maze/actor/actor.cpp maze/actor/actor.h
        maze/map/map.cpp maze/map/map.h
        vector/vector.cpp vector/vector.h
        maze/cell/cell.cpp maze/cell/cell.h
        maze/way/way.cpp maze/way/way.h)

set(CMAKE_EXE_LINKER_FLAGS "-static")
