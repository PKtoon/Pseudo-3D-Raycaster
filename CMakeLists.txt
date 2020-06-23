cmake_minimum_required(VERSION 3.5)

project(conShoot LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(SDL2 REQUIRED)

INCLUDE(FindPkgConfig)
PKG_SEARCH_MODULE(SDL2IMAGE REQUIRED SDL2_image>=2.0.0)

add_executable(conShoot main.cpp)
target_link_libraries(conShoot ${SDL2_LIBRARIES} ${SDL2IMAGE_LIBRARIES})
