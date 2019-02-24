cmake_minimum_required(VERSION 3.10)
project(GraphicsCWGame)

set(CMAKE_CXX_STANDARD 11)

add_executable(GraphicsCWGame main.cpp utility.cpp utility.h scene.cpp scene.h player.cpp player.h)
target_link_libraries(GraphicsCWGame -lglut -lSOIL -lGL -lGLU)