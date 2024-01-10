cmake_minimum_required (VERSION 2.8)
project(threadcleanup)
SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} --std=c++11 -g")

add_executable(threadcleanup threadcleanup.cpp)
target_link_libraries(threadcleanup pthread)
