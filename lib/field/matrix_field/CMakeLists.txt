add_library(matrix_field INTERFACE matrix_field.hpp)
add_library(matrix_opponent_field matrix_opponent_field.cpp matrix_opponent_field.hpp)
add_library(matrix_player_field matrix_player_field.cpp matrix_player_field.hpp)

target_include_directories(matrix_field INTERFACE ${PROJECT_SOURCE_DIR})
target_include_directories(matrix_opponent_field PUBLIC ${PROJECT_SOURCE_DIR})
target_include_directories(matrix_player_field PUBLIC ${PROJECT_SOURCE_DIR})

