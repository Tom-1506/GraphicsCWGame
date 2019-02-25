cmake_minimum_required(VERSION 3.10)
project(GraphicsCWGame)

set(CMAKE_CXX_STANDARD 11)

add_executable(GraphicsCWGame Main.cpp Utility.cpp Utility.h Scene.cpp Scene.h Player.cpp Player.h)
target_link_libraries(GraphicsCWGame -lglut -lSOIL -lGL -lGLU)