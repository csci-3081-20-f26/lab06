cmake_minimum_required(VERSION 3.5)
project(Viewer)

set(CMAKE_CXX_STANDARD 11) # no explicit compiler flags if possible
set(OpenGL_GL_PREFERENCE GLVND)
set(CMAKE_BUILD_TYPE Debug)

find_package(OpenGL REQUIRED)
find_package(PkgConfig REQUIRED)
# pkg_search_module(GLWF REQUIRED glfw3)
# find_package(glfw3 REQUIRED)

include_directories(
     ${OPENGL_INCLUDE_DIR}
     ${CMAKE_SOURCE_DIR}/lib 
     ${CMAKE_SOURCE_DIR}/lib/glm 
     ${CMAKE_SOURCE_DIR}/include 
     ${CMAKE_SOURCE_DIR}/include/graphics
     ${CMAKE_SOURCE_DIR}/lib/NanoGUI/install/include
     ${CMAKE_SOURCE_DIR}/lib/NanoGUI/src/ext/eigen
     ${CMAKE_SOURCE_DIR}/lib/NanoGUI/src/ext/nanovg/src
     ${CMAKE_SOURCE_DIR}/lib/NanoGUI/src/ext/glfw/include
)

link_directories(${CMAKE_SOURCE_DIR}/lib/NanoGUI/install/lib)

add_definitions(-DGLM_FORCE_INLINE)

file(GLOB_RECURSE MAP_PROJECT_SRC
     "main.cpp"
)

find_package(X11)

# Windows Section #
if (MSVC)
     add_definitions(-D_CRT_SECURE_NO_WARNINGS)
     # Tell MSVC to use main instead of WinMain for Windows subsystem executables
     set_target_properties(${WINDOWS_BINARIES} PROPERTIES LINK_FLAGS "/ENTRY:mainCRTStartup")
endif()

if (${CMAKE_SYSTEM_NAME} MATCHES "Darwin")
     set(CMAKE_XCODE_ATTRIBUTE_CLANG_CXX_LANGUAGE_STANDARD "c++11")
     set(CMAKE_XCODE_ATTRIBUTE_CLANG_CXX_LIBRARY "libc++")
     find_library(COCOA_LIB Cocoa)
     find_library(IOKIT_LIB IOKit)
     find_library(CORE_FOUNDATION_FRAMEWORK CoreFoundation)
          find_library(CORE_VIDEO_FRAMEWORK CoreVideo)
     set(DEP_LIBS ${DEP_LIBS} ${COCOA_LIB} ${IOKIT_LIB} ${CORE_FOUNDATION_FRAMEWORK} ${CORE_VIDEO_FRAMEWORK})
     message(STATUS "${CORE_VIDEO_FRAMEWORK}")
endif()

if (${CMAKE_SYSTEM_NAME} MATCHES "Linux")
     find_package(Threads)
     find_package(X11)
     set(DEP_LIBS ${DEP_LIBS} ${CMAKE_THREAD_LIBS_INIT} rt Xrandr Xxf86vm Xinerama Xcursor Xi m dl ${X11_LIBRARIES})
endif()

# Main executable
add_executable(Viewer ${MAP_PROJECT_SRC})
# target_link_libraries(Viewer ggis_graphics ${GLWF_LIBRARIES} nanogui.a OpenGL::GL ${DEP_LIBS})
target_link_libraries(Viewer ggis_graphics
    ${CMAKE_SOURCE_DIR}/lib/NanoGUI/install/lib/libnanogui.a
    OpenGL::GL ${DEP_LIBS})
add_dependencies(Viewer NanoGUI)
