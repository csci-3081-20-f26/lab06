cmake_minimum_required(VERSION 3.3)
project(ggis_core)

include_directories(${CMAKE_SOURCE_DIR}/include ${CMAKE_SOURCE_DIR}/lib ${CMAKE_SOURCE_DIR}/lib/glm)

add_definitions(-DGLM_FORCE_INLINE)

file(GLOB_RECURSE ${PROJECT_NAME}_PROJECT_SRC
     "main.cpp"
)

message( ${PROJECT_NAME}_PROJECT_SRC abc)

# Main executable
add_library(${PROJECT_NAME} ${${PROJECT_NAME}_PROJECT_SRC})
