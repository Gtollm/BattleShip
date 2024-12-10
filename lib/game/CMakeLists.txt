add_library(game game.cpp game.hpp)

target_link_libraries(game PUBLIC matrix_player_field matrix_opponent_field strategy ordered_strategy probabilistic_strategy)

target_include_directories(game PUBLIC ${PROJECT_SOURCE_DIR})
