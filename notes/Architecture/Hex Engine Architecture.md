# Hex Engine Architecture

The architecture is intentionally narrow right now so the project can expand
without a rewrite later.

## Layers

- [[Systems/Hex Simulation Core]] owns grid math, sparse storage, and future organism state.
- Rendering will eventually sit in a separate OpenGL layer.
- Tools and import/export live outside the runtime so editor work does not pollute the engine.
- Lua data will define rules and presets instead of hardcoding behavior into the engine.

## Goals

- Keep simulation deterministic.
- Keep the public engine surface small.
- Keep file formats versioned.
- Keep rendering decoupled from rules.

## Near-Term Shape

The first native slice is a small static library plus a demo binary.
That keeps the build simple while leaving room for [[Roadmap/Hex Engine Roadmap]] milestones.

