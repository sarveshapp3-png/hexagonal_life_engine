cmake_minimum_required(VERSION 3.20)

project(hex_engine VERSION 0.1.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

add_library(hex_engine_core STATIC)
target_sources(
    hex_engine_core
    PRIVATE
        src/world.cpp
        src/simulation.cpp
        src/organism.cpp
        src/mutation.cpp
        src/vision.cpp
        src/brain.cpp
)
target_include_directories(
    hex_engine_core
    PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}/include
)

add_executable(hex_engine_demo src/main.cpp)
target_link_libraries(hex_engine_demo PRIVATE hex_engine_core)

# Assets staging skipped.

if(MSVC)
    target_compile_options(hex_engine_core PRIVATE /W4 /permissive-)
    target_compile_options(hex_engine_demo PRIVATE /W4 /permissive-)
else()
    target_compile_options(hex_engine_core PRIVATE -Wall -Wextra -Wpedantic)
    target_compile_options(hex_engine_demo PRIVATE -Wall -Wextra -Wpedantic)
endif()
