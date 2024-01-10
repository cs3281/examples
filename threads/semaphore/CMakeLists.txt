cmake_minimum_required (VERSION 2.8)
project(threadcleanup)
SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} --std=c++11 -g")

add_executable(shm_consume shm_consume.cpp)
target_link_libraries(shm_consume pthread rt)

add_executable(shm_produce shm_produce.cpp)
target_link_libraries(shm_produce pthread rt)
