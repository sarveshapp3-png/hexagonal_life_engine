# Initial Scaffold

This note records the ultra-basic foundation that is now in the repo.

## Native Core

- `hex_engine/CMakeLists.txt`
- `hex_engine/include/hex_engine/hex_coord.h`
- `hex_engine/include/hex_engine/cell_kind.h`
- `hex_engine/include/hex_engine/world.h`
- `hex_engine/src/world.cpp`
- `hex_engine/src/main.cpp`

## Tooling

- `hex_engine/tools/HexSaveTool/HexSaveTool.csproj`
- `hex_engine/tools/HexSaveTool/Program.cs`
- `hex_engine/examples/sample.world.json`
- `hex_engine/scripts/default_rules.lua`

## Validation

- The C# save inspector builds successfully.
- The sample JSON world is readable by the tool.
- The C++ side is source-only for now because the local native compiler chain is not available on PATH in this environment.

