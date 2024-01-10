cmake_minimum_required (VERSION 2.6)
project(basicfork)
enable_testing()
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/bin")
add_executable(basicfork basicfork.c)

add_test(NAME basicfork COMMAND  basicfork WORKING_DIERCTORY ${CMAKE_CURRENT_SOURCE_DIR}/bin)
