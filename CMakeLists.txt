cmake_minimum_required(VERSION 3.13)
project(MonteCarlo)

set(CMAKE_CXX_STANDARD 14)

include_directories(MonteCarlo)

add_executable(MonteCarlo
        MonteCarlo/Camera.cpp
        MonteCarlo/Camera.h
        MonteCarlo/ColorDbl.cpp
        MonteCarlo/ColorDbl.h
        MonteCarlo/Direction.cpp
        MonteCarlo/Direction.h
        MonteCarlo/Pixel.cpp
        MonteCarlo/Pixel.h
        MonteCarlo/Ray.cpp
        MonteCarlo/Ray.h
        MonteCarlo/Scene.cpp
        MonteCarlo/Scene.h
        MonteCarlo/Triangle.cpp
        MonteCarlo/Triangle.h
        MonteCarlo/Vertex.cpp
        MonteCarlo/Vertex.h)
