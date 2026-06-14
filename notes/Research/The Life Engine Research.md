# The Life Engine Research

This note captures the reference behavior and layout of the original site.

## Source Site Observations

- The homepage presents a simulation-first dashboard.
- Major sections include Simulation Speed, Organism Details, World Controls, Evolution Controls, Statistics, and Community Creations.
- The UI uses a dark lab-console palette with compact cards and monospaced readouts.
- The interaction model is utility-driven rather than decorative.
- The page includes a visible title mark at the top, which we now preserve as a
  local branding reference asset.
- Community Creations is broken into Worlds, Organisms, and Mods.
- The status readouts emphasize live simulation information such as target FPS
  and population-style counters.

## Organism Model

- The original engine supports roles such as Mouth, Producer, Mover, Killer, Armor, and Eye.
- Organisms reproduce, mutate, move, rotate, and react to what they can see.
- The site exposes world controls, reset behaviors, mutation tuning, and statistics panels.

## Hex Adaptation Notes

- Use axial coordinates as the canonical runtime format.
- Use cube math where range, rotation, or line drawing needs a symmetric hex model.
- Keep movement to six directions.
- Keep the UI structure faithful while swapping the underlying lattice.
- Mirror the original section order before inventing new chrome so the remake
  still feels like the source.

## External References

- [The Life Engine](https://thelifeengine.net/)
- [LifeEngine GitHub](https://github.com/MaxRobinsonTheGreat/LifeEngine)
- [GIGAZINE coverage](https://gigazine.net/gsc_news/en/20230423-the-life-engine)
- [Red Blob Games Hexagonal Grids](https://www.redblobgames.com/grids/hexagons/)
- [2D Hexagonal Cellular Automata: The Complexity of the Forms](https://arxiv.org/abs/1811.12387)
- [Logic gates and complex dynamics in a hexagonal cellular automaton: the Spiral rule](https://arxiv.org/abs/1411.0784)
- [Simulations between triangular and hexagonal number-conserving cellular automata](https://arxiv.org/abs/0809.0355)

## Related Notes

- [[Project Brief The Life Engine Hex]]
- [[Architecture/Hex Engine Architecture]]
- [[Systems/Hex Simulation Core]]
