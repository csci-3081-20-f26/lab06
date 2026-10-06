cmake_minimum_required(VERSION 3.3)
project(GopherGIS)

set(CMAKE_CXX_STANDARD 11) # no explicit compiler flags if possible
set(CMAKE_BUILD_TYPE Debug)

add_subdirectory(src/core)
add_subdirectory(src/graphics)
add_subdirectory(apps/GopherMap)
add_subdirectory(apps/Viewer)