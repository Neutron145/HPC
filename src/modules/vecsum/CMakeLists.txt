add_library(vecsum STATIC
    vecsum.cpp
    vecsum.cu
)

target_link_libraries(vecsum PUBLIC core CUDA::cudart)

target_include_directories(vecsum PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})