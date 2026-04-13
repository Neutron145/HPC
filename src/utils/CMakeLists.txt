add_library(image_utils STATIC image.cpp)

target_include_directories(image_utils PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

target_link_libraries(image_utils PUBLIC easy_bmp)