# Foundation Decisions

These are the early decisions that keep the project from getting boxed in.

## Stack

- C++ for the simulation runtime.
- OpenGL for rendering.
- C# for authoring and inspection tools.
- Lua for data-driven rules, presets, and future mod hooks.
- The Lua runtime should be packaged with the project, not assumed to exist on
  every target laptop.

## Data Model

- Use axial hex coordinates as the canonical runtime coordinate system.
- Derive cube coordinates only when algorithms need them.
- Keep world storage sparse until density proves otherwise.

## Engineering Rules

- Keep modules isolated by responsibility.
- Prefer deterministic state transitions.
- Version all save data from the start.
- Keep comments focused on intent, invariants, and tradeoffs.
- Put portable runtime dependencies in the build or repo layout so a copied
  project still runs without extra installs.
