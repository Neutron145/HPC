add_library(spectogram STATIC
    spectogram.cpp
    spectogram.cu
    wav.cpp
    )


target_link_libraries(spectogram PUBLIC core CUDA::cudart)

target_include_directories(spectogram PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})