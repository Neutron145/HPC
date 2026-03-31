add_library(spectogram STATIC
    spectogram.cpp
    spectogram.cu
    wav.cpp
    )

find_package(PkgConfig REQUIRED)
pkg_check_modules(FFTWF REQUIRED IMPORTED_TARGET fftw3f)

target_link_libraries(spectogram PUBLIC core PkgConfig::FFTWF easy_bmp CUDA::cudart CUDA::cufft)

target_include_directories(spectogram PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})