add_library(spectogram STATIC
    spectogram.cpp
    spectogram.cu
    wav.cpp
    EasyBMP.cpp
    )

find_package(PkgConfig REQUIRED)
pkg_check_modules(FFTWF REQUIRED IMPORTED_TARGET fftw3f)

target_link_libraries(spectogram PUBLIC core PkgConfig::FFTWF)

target_include_directories(spectogram PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})