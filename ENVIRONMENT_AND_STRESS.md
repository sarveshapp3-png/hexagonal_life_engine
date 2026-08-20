# Hexagonal Life Engine: Environment & Stress Systems

This update introduces global environmental rules and specialized cell types that create a more challenging and dynamic evolutionary landscape, matching the core mechanics of the original Life Engine.

## 1. Global Food Spawning
The environment is no longer dependent solely on `Producer` cells for energy. A **Global Food Spawner** has been implemented:
- **Spawn Probability**: Configurable chance per tick to spawn food in an empty hex.
- **Energy Density**: Spawned food provides a significant energy boost to any `Mouth` or `Scavenger` that harvests it.
- **Selective Pressure**: This system encourages the evolution of "Gatherer" species that roam the map looking for resources.

## 2. Specialized Obstacles & Hazards
Two new cell types introduce physical and biological constraints to the world:

| Cell Kind | Function | Impact on Evolution |
|-----------|----------|---------------------|
| **Wall** | Indestructible Blocker | Prevents movement and reproduction into its hex. Organisms must evolve to navigate around terrain. |
| **Poison** | Lethal Hazard | When a `Mouth` harvests a Poison cell, the organism takes massive damage. Encourages the evolution of vision and discrimination. |

## 3. Implementation Details
- **Wall Collision**: Organisms now perform a "look-ahead" check during movement and reproduction. If any part of their anatomy would overlap with a `Wall`, the action is canceled.
- **Poison Metabolism**: Unlike standard `Food`, `Poison` cells trigger a `take_damage` call on the consumer, simulating a toxic reaction.

## 4. Parity with Hyperparameters
The engine now uses a `SimulationConfig` that mirrors the original `Hyperparameters.js`, including:
- `food_spawn_prob`
- `food_spawn_energy`
- `poison_damage`
- `explosive_radius`
- `virus_infection_chance`

---
### Technical Summary
- **Deterministic Spawning**: Uses a seeded PRNG for all global events.
- **Collision Efficiency**: Leveraging the **Chunk System** for rapid neighborhood checks during movement.
