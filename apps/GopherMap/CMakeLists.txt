cmake_minimum_required(VERSION 3.5)
project(GopherMap)

set(CMAKE_CXX_STANDARD 11) # no explicit compiler flags if possible
set(OpenGL_GL_PREFERENCE GLVND)
set(CMAKE_BUILD_TYPE Debug)

find_package(OpenGL REQUIRED)
find_package(PkgConfig REQUIRED)
# find_package(glfw3 REQUIRED)

include_directories(${OPENGL_INCLUDE_DIR})
include_directories(${CMAKE_SOURCE_DIR}/lib ${CMAKE_SOURCE_DIR}/lib/glm ${CMAKE_SOURCE_DIR}/include ${CMAKE_SOURCE_DIR}/include/graphics)

add_definitions(-DGLM_FORCE_INLINE)

file(GLOB_RECURSE MAP_PROJECT_SRC
     "main.cpp"
     "WebService.cpp"
)

# Main executable
add_executable(GopherMap ${MAP_PROJECT_SRC})

if (APPLE)
    # macOS: use the GLFW that NanoGUI builds into libnanogui.a
    target_include_directories(GopherMap PRIVATE
        ${CMAKE_SOURCE_DIR}/lib/NanoGUI/src/ext/glfw/include
    )
    target_link_libraries(GopherMap
        ggis_graphics
        ${CMAKE_SOURCE_DIR}/lib/NanoGUI/install/lib/libnanogui.a
        OpenGL::GL
        curl
        "-framework Cocoa"
        "-framework IOKit"
        "-framework CoreFoundation"
        "-framework CoreVideo"
    )
else()
    # Linux: system GLFW from pkg-config (original setup)
    pkg_search_module(GLWF REQUIRED glfw3)
    target_link_libraries(GopherMap ggis_graphics OpenGL::GL ${GLWF_LIBRARIES} curl)
endif()

add_dependencies(GopherMap NanoGUI)
