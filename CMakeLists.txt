cmake_minimum_required(VERSION 3.5)

project(raycaster LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_executable(raycaster main.cpp)

target_link_libraries(raycaster -lsfml-graphics -lsfml-window -lsfml-system)
