# Run Instructions

This note records the current entry points for the project.

## Native Engine

- The main native source entry point is `hex_engine/src/main.cpp`.
- The native CMake target is `hex_engine_demo`.
- The current native binary is still a console sandbox, not a GUI yet.
- The official Life Engine title mark is staged at
  `hex_engine/assets/reference/life-engine-title.png`.
- To run the full engine, build the C++ project in `hex_engine/` with a machine
  that has CMake and a native compiler toolchain installed.

## C# Tooling

- The runnable C# tool lives in `hex_engine/tools/HexSaveTool/Program.cs`.
- Run it from `hex_engine/` with `dotnet run --project tools/HexSaveTool -- examples/sample.world.json`.
- It can inspect `hex_engine/examples/sample.world.json`.

## Lua

- Lua is a future rules runtime, not a required laptop-wide install.
- Local Lua is helpful for script testing, but the project should bundle the
  runtime when we start shipping real Lua-driven gameplay.

## Current Reality

- The workspace now has the clean root project in [[Overview]] and the
  documentation vault in [[Project Brief The Life Engine Hex]].
- The branding asset is recorded in [[Branding/Life Engine Logo]].
- The stitch folder is back as reference material, but the active project still
  lives in `hex_engine/`.
