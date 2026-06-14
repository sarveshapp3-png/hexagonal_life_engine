# Lua Runtime Strategy

This note records how Lua should be handled so the project stays portable.

## Laptop

- Install Lua locally only if you want to run or debug scripts directly.
- Do not require Lua to be present just to build the native engine.

## Project

- Keep gameplay and rules scripts under `hex_engine/scripts/`.
- Package the Lua runtime with the project when we add real scripting support.
- Prefer a vendored dependency or build-system fetch over relying on a global
  machine install.

## Shipping

- Other laptops should be able to run the engine without manually installing
  Lua first.
- That means the executable should carry the Lua runtime, or the build should
  statically link it where practical.

## Why

- This avoids the "works on my machine" problem.
- It keeps the rules system portable for teammates and future users.

## Related Notes

- [[Foundation Decisions]]
- [[Project Brief The Life Engine Hex]]
- [[Checklist/Laptop And Project Setup]]
- [[Usage/Run Instructions]]
