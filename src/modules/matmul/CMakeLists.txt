add_library(matmul STATIC
    matmul.cpp
    matmul.cu
)

target_link_libraries(matmul PUBLIC core CUDA::cudart)

target_include_directories(matmul PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
