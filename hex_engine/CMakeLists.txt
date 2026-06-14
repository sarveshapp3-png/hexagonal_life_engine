cmake_minimum_required(VERSION 3.20)

# Keep the project name explicit so future C# tooling, Lua assets, and native
# rendering targets can sit beside the core engine without guesswork.
project(hex_engine VERSION 0.1.0 LANGUAGES CXX)

# We want a modern, predictable dialect for the first milestone.
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# A tiny core library gives us a stable seam for simulation, rendering, and
# tooling to link against later.
add_library(hex_engine_core STATIC)
target_sources(
    hex_engine_core
    PRIVATE
        src/world.cpp
)
target_include_directories(
    hex_engine_core
    PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}/include
)

# The demo executable is intentionally plain for now; it proves the model and
# keeps the first iteration easy to reason about.
add_executable(hex_engine_demo src/main.cpp)
target_link_libraries(hex_engine_demo PRIVATE hex_engine_core)

# Reference art and future UI assets should travel with the executable so the
# source tree and build tree expose the same file layout.
set(HEX_ENGINE_ASSET_SOURCE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/assets)
add_custom_command(
    TARGET hex_engine_demo
    POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
            ${HEX_ENGINE_ASSET_SOURCE_DIR}
            $<TARGET_FILE_DIR:hex_engine_demo>/assets
    COMMENT "Staging reference assets beside hex_engine_demo"
)

# Warnings stay strict so we catch layout and API mistakes early.
if(MSVC)
    target_compile_options(hex_engine_core PRIVATE /W4 /permissive-)
    target_compile_options(hex_engine_demo PRIVATE /W4 /permissive-)
else()
    target_compile_options(hex_engine_core PRIVATE -Wall -Wextra -Wpedantic)
    target_compile_options(hex_engine_demo PRIVATE -Wall -Wextra -Wpedantic)
endif()
