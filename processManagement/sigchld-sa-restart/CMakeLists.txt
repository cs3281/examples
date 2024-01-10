cmake_minimum_required (VERSION 2.6)
project(basicsigchld-sigaction)
#SET(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -std=c99")
add_executable(basicsigchld-sigaction sigchld-sigaction.cpp)
add_executable(basicsigchld-sigaction-sa-restart sigchld-sigaction-sa-restart.cpp)

