add_library(field INTERFACE field.hpp)

add_subdirectory(matrix_field)

target_include_directories(field INTERFACE ${PROJECT_SOURCE_DIR})

