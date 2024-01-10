cmake_minimum_required (VERSION 2.6)
project(basicwait)
enable_testing()
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/bin")
add_executable(basicwait basicwait.c)

add_test(NAME basicwait COMMAND  basicwait WORKING_DIERCTORY ${CMAKE_CURRENT_SOURCE_DIR}/bin)