# Hexagonal Life Engine Workspace

This repository is split into three things:

- `hex_engine/` - the active native project.
- `notes/` - the Obsidian vault that explains the project.
- `stitch_hexagonal_life_engine/` - reference material only.

## Read This First

If you are a new human or a new AI entering the workspace, start here:

1. `notes/Overview.md`
2. `notes/Project Brief The Life Engine Hex.md`
3. `notes/Checklist/Laptop And Project Setup.md`
4. `notes/Decisions/Foundation Decisions.md`
5. `notes/Research/The Life Engine Research.md`
6. `hex_engine/README.md`

The notes are not optional. They explain the architecture, research, roadmap,
branding, setup, and the decisions that keep the project from drifting.
Reading them will help a lot.

## First 5 Commands

Run these from PowerShell in order:

1. `Set-Location C:\hexagonal_life_engine`
2. `Get-Content .\notes\Overview.md`
3. `Get-Content .\notes\Project Brief The Life Engine Hex.md`
4. `Get-Content .\notes\Checklist\Laptop And Project Setup.md`
5. `dotnet run --project .\hex_engine\tools\HexSaveTool -- .\hex_engine\examples\sample.world.json`

If the C# tool prints the sample world summary, the workspace is wired up
correctly. If you want to build the native C++ side next, make sure CMake and a
Windows C++ toolchain are installed first.

## Current Project Shape

- C++ is the main simulation/runtime language.
- OpenGL is the future rendering layer.
- C# is used for inspection and tooling.
- Lua is planned for data-driven rules and should be bundled with the project
  when it becomes part of the shipped experience.
- The official Life Engine title logo is stored at
  `hex_engine/assets/reference/life-engine-title.png`.

## What Is Already Here

- A small axial-hex foundation in C++.
- A console demo that proves the world and hex math.
- A C# save-file inspector.
- A sample JSON world file.
- A Lua rules seam for later.
- A branding note and a setup checklist in `notes/`.

## What Still Comes Next

- A real OpenGL window.
- The first visible GUI shell.
- Hexagonal cellular automata rules.
- Rendering, editor, and world persistence.

## Laptop Setup

Install these on the laptop:

- A C++ compiler toolchain. On Windows, use MSVC Build Tools or a similar
  toolchain that CMake can find.
- CMake.
- Git.
- VS Code with C/C++ and CMake support.
- .NET if you want to run the C# inspector.

Lua does not need to be a laptop-wide install for the project to be portable.
We should bundle or vendor Lua with the project when we add the real scripting
runtime.

## Reference Logo

The official Life Engine title mark is kept as a local reference asset, not as
the final UI system:

- `hex_engine/assets/reference/life-engine-title.png`

## Quick Run Notes

- The current native C++ entry point is still a console demo.
- The C# tool can be run with:
  - `dotnet run --project hex_engine/tools/HexSaveTool -- hex_engine/examples/sample.world.json`

## Design Notes

The project is aiming for a synthetic-lab look:

- dark surfaces
- neon accents
- compact dashboards
- technical, data-heavy controls

The detailed design direction is documented in `notes/Design/`.
