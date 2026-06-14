# Hex Engine

This is the root-level native project for the hexagonal Life Engine remake.

![The Life Engine title logo](assets/reference/life-engine-title.png)

See also:
- [[notes/Overview]]
- [[notes/Architecture/Hex Engine Architecture]]
- [[notes/Systems/Hex Simulation Core]]
- [[notes/Roadmap/Hex Engine Roadmap]]

What exists right now:
- A small C++20 core library.
- Axial hex coordinate math with deterministic helpers.
- Sparse world storage using hashed hex coordinates.
- A console demo that prints a tiny seeded world.
- A Lua config seam for the future rule system.
- A tiny C# save-file inspector that proves the toolchain and data contract.
- A sample JSON world snapshot under `examples/` for the save inspector.
- The official Life Engine title mark in `assets/reference/life-engine-title.png`.

What does not exist yet:
- OpenGL rendering.
- The organism rule engine.
- UI panels, editor tools, or persistence.

How we are building this:
- Keep the first layer tiny and readable.
- Add one feature seam at a time.
- Prefer data-driven rules and deterministic world state from the start.
- Leave clear boundaries between simulation, tools, and rendering so each one
  can grow independently later.
- Keep the reference logo asset staged beside the demo executable so the first
  OpenGL shell can reuse it without changing the source tree layout.

How to run it:
- Native entry point: `src/main.cpp`
- Native build target: `hex_engine_demo`
- Build system: CMake in this folder
- C# utility entry point: `tools/HexSaveTool/Program.cs`
- C# utility run command: `dotnet run --project tools/HexSaveTool -- examples/sample.world.json`

First 5 commands from inside `hex_engine/`:
1. `Set-Location C:\hexagonal_life_engine\hex_engine`
2. `Get-Content ..\notes\Overview.md`
3. `Get-Content ..\notes\Project Brief The Life Engine Hex.md`
4. `Get-Content ..\notes\Checklist\Laptop And Project Setup.md`
5. `dotnet run --project tools\HexSaveTool -- examples\sample.world.json`

If you are on a machine with a native C++ compiler and CMake installed, build
the C++ project from `hex_engine/` and run the generated `hex_engine_demo`
executable.

If you just want something runnable right now in this workspace, the C# save
inspector is already buildable and can inspect `examples/sample.world.json`.

The goal of this first step is to keep the foundation simple, testable, and easy
to extend without forcing a rewrite later.
