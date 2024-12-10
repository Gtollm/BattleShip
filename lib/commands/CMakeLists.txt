add_library(commands commands.cpp commands.hpp)

target_link_libraries(commands PUBLIC game)

target_include_directories(commands PUBLIC ${PROJECT_SOURCE_DIR})
