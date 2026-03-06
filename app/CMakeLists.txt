add_executable(HPC main.cpp)

target_link_libraries(HPC PRIVATE matmul vecsum)
