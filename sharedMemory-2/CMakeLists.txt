add_executable(shm_r shm_r.cpp)
target_link_libraries(shm_r pthread rt)
add_executable(shm_s shm_s.cpp)
target_link_libraries(shm_s pthread rt)
