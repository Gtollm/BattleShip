add_library(game_manager command_processors.cpp command_processors.hpp game_manager.cpp game_manager.hpp )

target_link_libraries(game_manager PUBLIC game)
target_link_libraries(game_manager PUBLIC writer)
target_link_libraries(game_manager PUBLIC commands)

target_include_directories(game_manager PUBLIC ${PROJECT_SOURCE_DIR})
