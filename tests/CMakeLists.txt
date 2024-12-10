include(FetchContent)

FetchContent_Declare(
        googletest
        GIT_REPOSITORY https://github.com/google/googletest.git
        GIT_TAG release-1.12.1
)

# For Windows: Prevent overriding the parent project's compiler/linker settings
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)

enable_testing()

add_executable(
        battleship_tests
        packed_array_test.cpp
        packed_vector_test.cpp
        packed_array_matrix_test.cpp
        packed_vector_matrix_test.cpp
        field_test.cpp
        game_manager_test.cpp
)

target_link_libraries(
        battleship_tests
        packed_array
        packed_vector
        packed_matrix
matrix_player_field matrix_opponent_field
        game_manager
        GTest::gtest_main
)

target_include_directories(battleship_tests PUBLIC ${PROJECT_SOURCE_DIR})

include(GoogleTest)

gtest_discover_tests(battleship_tests)
