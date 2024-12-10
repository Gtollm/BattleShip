add_library(packed_array INTERFACE packed_array.hpp)
add_library(packed_vector INTERFACE packed_vector.hpp)
add_library(packed_matrix INTERFACE packed_matrix.hpp packed_matrix.tpp)

target_include_directories(packed_array INTERFACE ${PROJECT_SOURCE_DIR})
target_include_directories(packed_vector INTERFACE ${PROJECT_SOURCE_DIR})
target_include_directories(packed_matrix INTERFACE ${PROJECT_SOURCE_DIR})
