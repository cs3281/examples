cmake_minimum_required (VERSION 2.8)
project(condvar_example)
SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} --std=c++11 -g")

add_executable(condvar condvar.c)
target_link_libraries(condvar pthread)
