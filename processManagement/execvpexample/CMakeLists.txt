cmake_minimum_required (VERSION 2.6)
project(execvpexample)
enable_testing()
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/bin")
add_executable(execvpexample execvpexample.c)


add_test(NAME execvpexample COMMAND  execvpexample WORKING_DIERCTORY ${CMAKE_CURRENT_SOURCE_DIR}/bin)