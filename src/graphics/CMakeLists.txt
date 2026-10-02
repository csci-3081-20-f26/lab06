cmake_minimum_required(VERSION 3.3)
project(ggis_graphics)

set(CMAKE_CXX_STANDARD 11) # no explicit compiler flags if possible
set(OpenGL_GL_PREFERENCE GLVND)
set(CMAKE_BUILD_TYPE Debug)

find_package(OpenGL REQUIRED)
find_package(PkgConfig REQUIRED)
pkg_search_module(GLWF REQUIRED glfw3)

include_directories(${OPENGL_INCLUDE_DIR})
include_directories(${CMAKE_SOURCE_DIR}/lib ${CMAKE_SOURCE_DIR}/lib/glm ${CMAKE_SOURCE_DIR}/include ${CMAKE_SOURCE_DIR}/include/graphics)

add_definitions(-DGLM_FORCE_INLINE)

file(GLOB_RECURSE ggis_graphics_PROJECT_SRC
     "Quad.cpp"
     "ShaderProgram.cpp"
     "Texture.cpp"
     "Glyph.cpp"
     "Node.cpp"
     "Image.cpp"
     "${CMAKE_SOURCE_DIR}/lib/glad/glad.c"
)

add_library(${PROJECT_NAME} ${ggis_graphics_PROJECT_SRC})

