cmake_minimum_required (VERSION 2.8)
project (lecture-7-examples)
enable_testing()
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/bin")
add_executable(memoryerror memoryerror.c)
add_executable(notifychild notifychild.c)  

add_test(NAME memoryerror COMMAND  memoryerror WORKING_DIERCTORY ${CMAKE_CURRENT_SOURCE_DIR}/bin)
add_test(NAME notifychild COMMAND  notifychild WORKING_DIERCTORY ${CMAKE_CURRENT_SOURCE_DIR}/bin)