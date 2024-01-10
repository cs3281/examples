cmake_minimum_required (VERSION 2.6)
project(basicexec)
enable_testing()
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/bin")
add_executable(basicexec basicexec.c)

add_test(NAME basicexec COMMAND  basicexec WORKING_DIERCTORY ${CMAKE_CURRENT_SOURCE_DIR}/bin)