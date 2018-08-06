
cmake_minimum_required(VERSION 3.10.2)
project(earlyman)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

set( CMAKE_VERBOSE_MAKEFILE on )

# includes cmake/FindSDL2.cmake
set(CMAKE_MODULE_PATH ${PROJECT_SOURCE_DIR}/cmake)

find_package(SDL2 REQUIRED)
include_directories(${SDL2_INCLUDE_DIR})

find_package(OpenGL REQUIRED)
include_directories(${OPENGL_INCLUDE_DIR})

find_package(glfw3 3.2 REQUIRED)
#find_package(GLEW 2.0 REQUIRED)

set(RAPIDJSON_USE_SSE2 ON)
set(RAPIDJSON_INCLUDEDIR "/Users/jharding/rapidjson/include")

find_package(rapidjson REQUIRED)

include_directories("${RAPIDJSON_INCLUDE_DIRS}")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${RAPIDJSON_CXX_FLAGS}")

set(SOURCE_FILES src/main.cpp)

add_executable(earlyman ${SOURCE_FILES})
target_link_libraries(earlyman ${OPENGL_LIBRARIES} ${GLEW_LIBRARIES} ${SDL2_LIBRARY} glfw)
