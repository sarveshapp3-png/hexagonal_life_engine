# Life Engine Logo

This note records the official title mark from the source project so we can
reuse it as a local reference asset without mixing it into the future synthetic
lab visual system.

## Source

- Original asset: https://thelifeengine.net/img/title.png

## Local Copy

- `hex_engine/assets/reference/life-engine-title.png`

## Current Use

- The image is displayed in `hex_engine/README.md`.
- The native demo prints the staged asset path for quick verification.
- The CMake build copies `assets/` next to the demo executable so the first
  OpenGL shell can load the same file later.

## Why It Is Separate

- The reference logo stays distinct from the eventual in-game UI branding.
- Future renderer code can load the exact same asset path in both source-tree
  and build-tree layouts.

## Related Notes

- [[Project Brief The Life Engine Hex]]
- [[Research/The Life Engine Research]]
- [[Design/Synthetic Lab Design]]
- [[Checklist/Laptop And Project Setup]]
