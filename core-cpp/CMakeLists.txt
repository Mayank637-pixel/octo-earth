cmake_minimum_required(VERSION 3.20)
project(octocore VERSION 0.1.0 LANGUAGES CXX C)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_POSITION_INDEPENDENT_CODE ON)



add_compile_options(-O3 -ffast-math -Wall -Wextra)

add_library(octocore SHARED
        src/c_api_bridge.cpp
)

target_include_directories(octocore PUBLIC
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/../contracts>
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
)

enable_testing()
add_executable(test_sanity tests/bench_10k_years.cpp)
target_link_libraries(test_sanity PRIVATE octocore)
add_test(NAME test_sanity COMMAND test_sanity)