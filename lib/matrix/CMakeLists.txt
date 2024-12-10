add_library(matrix INTERFACE abstract_matrix.hpp vector_matrix.hpp)

target_link_libraries(matrix INTERFACE packed_array)
