add_library(model STATIC expression.cpp parser.cpp)

target_include_directories(model PUBLIC ${PROJECT_SOURCE_DIR}/include/model  ${PROJECT_SOURCE_DIR}/include/misc)