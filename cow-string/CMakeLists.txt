cmake_minimum_required(VERSION 3.13 FATAL_ERROR)
project(cow-string)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -pedantic -Wall -Wextra -Werror -Wshadow")

# --------------------------google-tests--------------------------------------

enable_testing()
find_package(GTest REQUIRED)

add_executable(unit_tests tests/tests.cpp)

target_link_libraries(unit_tests PUBLIC GTest::gtest_main)
target_include_directories(unit_tests PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

add_custom_target(run_unit_tests
    COMMENT "Running unit tests"
    COMMAND ${CMAKE_CURRENT_BINARY_DIR}/unit_tests --gtest_color=yes
)
add_dependencies(run_unit_tests unit_tests)
