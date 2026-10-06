cmake_minimum_required(VERSION 3.3)
project(ggis_core)

include_directories(${CMAKE_SOURCE_DIR}/include ${CMAKE_SOURCE_DIR}/lib ${CMAKE_SOURCE_DIR}/lib/glm)

include(ExternalProject)
ExternalProject_add(glm
     GIT_REPOSITORY https://github.com/g-truc/glm
     GIT_TAG 1.0
     SOURCE_DIR ${CMAKE_SOURCE_DIR}/lib/glm
     UPDATE_COMMAND ""
     BUILD_COMMAND ""
     CONFIGURE_COMMAND ""
     INSTALL_COMMAND ""
)
add_definitions(-DGLM_FORCE_INLINE)

file(GLOB_RECURSE ${PROJECT_NAME}_PROJECT_SRC
     "main.cpp"
)

# Main executable
add_library(${PROJECT_NAME} ${${PROJECT_NAME}_PROJECT_SRC})
