# Hex Simulation Core

The core system is the first real code path in the project.

## What Exists

- Axial coordinates with derived cube space in `hex_coord.h`.
- Six-direction neighbor lookup.
- Hex distance calculation.
- Sparse world storage keyed by hex coordinate.
- Cell role enumeration for the Life Engine organisms.

## Why This Shape

- Axial coordinates are compact and easy to store.
- Cube math remains available through the derived `s` coordinate.
- Sparse storage avoids forcing a dense array before we know the final world size.
- Stable coordinate ordering helps later with saves, replays, and tests.

## Future Additions

- Tick stepping.
- Reproduction and mutation.
- Line of sight and brain logic.
- Wraparound or bounded world modes.
- Chunked storage for larger simulations.

