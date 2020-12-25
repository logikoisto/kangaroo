cmake_minimum_required(VERSION 3.0)

project(kangaroo CXX)

set(CXX_FLAGS
 -O2
 -g
 -Wall
 -Wextra
 -Werror
 -std=c++14
 -pthread
)

# ---[ Flags
string(REPLACE ";" " " CMAKE_CXX_FLAGS "${CXX_FLAGS}")

# ---[ Includes
include_directories(${PROJECT_SOURCE_DIR}/src)
include_directories(${PROJECT_SOURCE_DIR}/include)
include_directories(${PROJECT_SOURCE_DIR}/src/util)

# --[ Output directory
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/libs)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/libs)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

# ---[ Subdirectories
add_subdirectory(src)
add_subdirectory(test)


message(STATUS "CMAKE_CXX_FLAGS=" "${CMAKE_CXX_FLAGS}")

message(STATUS "PROJECT_BINARY_DIR=" "${PROJECT_BINARY_DIR}")

message(STATUS "PROJECT_SOURCE_DIR=" "${PROJECT_SOURCE_DIR}")

message(STATUS "CMAKE_BINARY_DIR=" "${CMAKE_BINARY_DIR}")