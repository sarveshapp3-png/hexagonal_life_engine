# Laptop And Project Setup

This checklist splits the practical setup into two buckets: what belongs on the
laptop and what belongs in the project tree.

## On Your Laptop

- [ ] Microsoft C++ Build Tools or the Visual Studio C++ desktop workload.
- [ ] MSVC v143 toolset and the Windows 10/11 SDK.
- [ ] CMake 3.20 or newer.
- [ ] Git for source control.
- [ ] VS Code extensions: C/C++ and CMake Tools.
- [ ] .NET SDK 9 if you want to run the C# save inspector.
- [ ] Lua only if you want to run or debug scripts directly on your laptop.
- [ ] OpenGL windowing libraries later, once we choose the GUI stack.

## In The Project Path

- [x] `hex_engine/CMakeLists.txt`
- [x] `hex_engine/include/hex_engine/hex_coord.h`
- [x] `hex_engine/include/hex_engine/cell_kind.h`
- [x] `hex_engine/include/hex_engine/world.h`
- [x] `hex_engine/src/world.cpp`
- [x] `hex_engine/src/main.cpp`
- [x] `hex_engine/tools/HexSaveTool/`
- [x] `hex_engine/scripts/default_rules.lua`
- [x] `hex_engine/examples/sample.world.json`
- [x] `hex_engine/assets/reference/life-engine-title.png`
- [ ] `hex_engine/third_party/` or a package-managed Lua dependency when we add
  the script runtime
- [x] `notes/`
- [ ] Native OpenGL renderer
- [ ] World persistence and editor flows
- [ ] Full cellular automaton tick loop

## Quick Read

- If a dependency is a compiler, SDK, or runtime, it belongs on the laptop.
- If a dependency is project data, source, notes, or branding art, it belongs
  under `hex_engine/` or `notes/`.
- If a dependency must be portable for other laptops, prefer bundling it with
  the project instead of asking each person to install it manually.
