cmake_minimum_required (VERSION 2.8)
project (fileexamples)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/bin")

add_executable(creat creat.c)
add_executable(stat stat.c)
add_executable(delete delete.c)
add_executable(flock flock.c)
add_executable(grow grow.c)
add_executable(readdir readdir.c)
add_executable(opendir opendir.c)
add_executable(trunc trunc.c)
