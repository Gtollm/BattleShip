add_library(strategy INTERFACE strategy.hpp)

target_include_directories(strategy INTERFACE ${PROJECT_SOURCE_DIR})

add_subdirectory(ordered_strategy)
add_subdirectory(probabilistic_strategy)
