cmake_minimum_required(VERSION 3.9)

project(MazeCreator VERSION 0.1 LANGUAGES C)

set(CMAKE_C_FLAGS "-Wall")
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${PROJECT_SOURCE_DIR}/bin")
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY "${PROJECT_SOURCE_DIR}/lib")

set(CMAKE_EXPORT_COMPILE_COMMANDS ON CACHE INTERNAL "")

add_library(MazeViewer STATIC lib/MazeViewer.c)
add_library(MazeTools STATIC lib/MazeTools.c)

add_executable(MazeCreator src/main.c)

target_include_directories(MazeCreator PUBLIC lib)
target_link_libraries(MazeCreator MazeViewer MazeTools)
