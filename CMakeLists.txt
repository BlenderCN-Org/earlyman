cmake_minimum_required(VERSION 3.10.2)
project(earlyman)

# includes cmake/FindSDL2.cmake
set(CMAKE_MODULE_PATH ${PROJECT_SOURCE_DIR}/cmake)

find_package(SDL2 REQUIRED)
include_directories(${SDL2_INCLUDE_DIR})

find_package(OpenGL REQUIRED)
include_directories(${OPENGL_INCLUDE_DIR})

find_package(GLEW 2.0 REQUIRED)

set(SOURCE_FILES src/main.cpp)

add_executable(earlyman ${SOURCE_FILES})
target_link_libraries(earlyman ${OPENGL_LIBRARIES} ${GLEW_LIBRARIES} ${SDL2_LIBRARY})
