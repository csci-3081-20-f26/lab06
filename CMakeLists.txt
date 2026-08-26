cmake_minimum_required(VERSION 3.3)
project(GLProject)

set(CMAKE_CXX_STANDARD 11) # no explicit compiler flags if possible
set(OpenGL_GL_PREFERENCE GLVND)
set(CMAKE_BUILD_TYPE Debug)

find_package(OpenGL REQUIRED)
find_package(PkgConfig REQUIRED)
pkg_search_module(GLWF REQUIRED glfw3)
# find_package(glfw3 REQUIRED)

include_directories(${OPENGL_INCLUDE_DIR})
include_directories(lib)

file(GLOB_RECURSE GL_PROJECT_SRC
     "apps/*.cpp"
     "src/*.cpp"
     "lib/*.cpp"
     "lib/glad/*.c"
)

# Main executable
add_executable(GLProject ${GL_PROJECT_SRC})
target_link_libraries(GLProject OpenGL::GL ${GLWF_LIBRARIES})

