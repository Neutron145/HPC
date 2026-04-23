add_library(bilateral STATIC
    bilateral.cpp
    bilateral.cu
    )

target_link_libraries(bilateral PUBLIC image_utils core CUDA::cudart)

target_include_directories(bilateral PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})