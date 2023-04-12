add_library(misc STATIC utils.cpp)

target_include_directories(misc PUBLIC ${PROJECT_SOURCE_DIR}/include/misc)