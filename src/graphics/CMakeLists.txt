cmake_minimum_required(VERSION 3.5)
project(ggis_graphics)

set(CMAKE_CXX_STANDARD 11) # no explicit compiler flags if possible
set(OpenGL_GL_PREFERENCE GLVND)
set(CMAKE_BUILD_TYPE Debug)

find_package(OpenGL REQUIRED)
find_package(PkgConfig REQUIRED)
pkg_search_module(GLWF REQUIRED glfw3)

include(ExternalProject)
set(NanoGUIArgs -DNANOGUI_BUILD_SHARED=OFF -DNANOGUI_BUILD_PYTHON=OFF -DNANOGUI_BUILD_EXAMPLE=OFF -DCMAKE_INSTALL_PREFIX=${CMAKE_SOURCE_DIR}/lib/NanoGUI/install -DCMAKE_C_FLAGS=-fPIC -DCMAKE_CXX_FLAGS=-fPIC  -DCMAKE_DEBUG_POSTFIX=d -DCMAKE_CONFIGURATION_TYPES='Debug Release MinSizeRel RelWithDebInfo')
ExternalProject_add(NanoGUI
     GIT_REPOSITORY https://github.com/wjakob/nanogui.git
     CMAKE_ARGS ${NanoGUIArgs}
     SOURCE_DIR ${CMAKE_SOURCE_DIR}/lib/NanoGUI/src
     BINARY_DIR ${CMAKE_SOURCE_DIR}/lib/NanoGUI/build
     INSTALL_DIR ${CMAKE_SOURCE_DIR}/lib/NanoGUI/install
     UPDATE_COMMAND ""
)

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

