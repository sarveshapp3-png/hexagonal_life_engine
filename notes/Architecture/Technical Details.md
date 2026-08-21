# Hexagonal Life Engine: Technical Deep Dive

This document outlines the architectural enhancements implemented to bring the **Hexagonal Life Engine** into functional parity with the original [thelifeengine.net](https://thelifeengine.net) while optimizing for hexagonal geometry and performance.

## 1. Advanced Brain & Behavioral Logic
The organism's decision-making process now strictly follows the original engine's "Reaction Mapping" model. Every organism possesses a `BrainParams` structure that maps observed `CellKind` values to specific `Decisions`:
- **Neutral (0)**: No action taken.
- **Chase (1)**: Move towards the detected cell.
- **Retreat (2)**: Move away from the detected cell.

This mapping evolves through **Brain Mutations** during reproduction, allowing species to develop complex survival strategies, such as predators that chase producers and prey that retreats from killers.

## 2. Species Tracking & Fossil Record
To simulate true evolution, the engine now includes a robust **Fossil Record** system:
- **Species Identification**: A new `Species` is automatically recorded whenever a mutation occurs during reproduction.
- **Population Tracking**: The `FossilRecord` monitors the population of every extant species in real-time.
- **Extinction Handling**: When a species' population hits zero, it is moved to the extinct list, preserving its genetic history for future analysis.

## 3. Spatial Partitioning (Chunk System)
To maintain high performance as the simulation scales to thousands of cells, the world is now divided into **Chunks** (radius of 16 hexes):
- **Sparse Storage**: Only active regions of the world consume memory.
- **Optimized Lookups**: Neighborhood queries, vision raycasting, and collision detection now only check local chunks rather than the entire world map.
- **Deterministic Diffusion**: Pheromone diffusion is processed per-chunk with a double-buffering approach to ensure cross-chunk consistency.

## 4. Genetic Distance
A `Genome::distance` function has been implemented to quantify the evolutionary gap between organisms. This function calculates:
- **Anatomy Distance**: Based on the difference in cell counts and physical layout.
- **Brain Distance**: Based on the number of mismatched behavioral reactions.

## 5. Biological Parity Summary
The following table summarizes the parity between this hexagonal implementation and the original square-grid logic:

| Feature | thelifeengine.net | Hexagonal Life Engine |
|---------|-------------------|-----------------------|
| **Grid** | Square | Hexagonal (Axial) |
| **Movement** | 4-way | 6-way |
| **Reproduction** | Asexual Mutation | Asexual Mutation + Genome Distance |
| **Energy** | Shared Pool | Shared Pool (Unified) |
| **Vision** | Raycasting | Raycasting (Hex-optimized) |
| **Brain** | Reaction Map | Reaction Map (Evolvable) |
| **Performance** | Fixed Array | Spatial Chunks (Scalable) |

---
### References
- [1] [MaxRobinsonTheGreat/LifeEngine Source Code](https://github.com/MaxRobinsonTheGreat/LifeEngine)
- [2] [Hexagonal Life Engine GitHub](https://github.com/sarveshapp3-png/hexagonal_life_engine)
