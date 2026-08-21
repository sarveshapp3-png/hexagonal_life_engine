# Hexagonal Life Engine: Sexual Reproduction & Dynamic Disasters

This update brings the engine to its most advanced state yet, introducing complex biological interactions and environmental instability that mirror the most popular "Hardcore" and "Realism" mods of the original Life Engine.

## 1. Sexual Reproduction (Gene Crossover)
Organisms are no longer limited to asexual cloning. They can now engage in **Sexual Reproduction**:
- **Mate Selection**: Organisms scan their immediate neighborhood for potential mates.
- **Kinship Recognition**: Mating only occurs between organisms of the same species or those within a specific **Genetic Distance (Kinship Threshold)**.
- **Genome Crossover**: The offspring's genome is a combination of both parents' anatomy and neural synapses, allowing for rapid trait recombination and the emergence of hybrid strategies.

## 2. Dynamic Disaster System
The environment is now prone to sudden, world-altering events that test the resilience of every species:

| Disaster Type | Effect | Evolutionary Pressure |
|---------------|--------|-----------------------|
| **Radiation** | 5x Mutation Rate | Forces rapid adaptation; high risk of extinction but potential for major breakthroughs. |
| **Blight** | 80% Energy Reduction for Producers | Favors predators and scavengers over self-sustaining producers. |
| **Solar Flare** | Vision Range reduced to 0 | Blinds all organisms; favors those with random-walk or pheromone-based navigation. |

## 3. Modern C++20 Architecture
The core engine has been refactored to utilize modern C++20 features:
- **Three-Way Comparison (`<=>`)**: Implemented for `HexCoord` to ensure efficient sorting and map lookups.
- **Consistent `[[nodiscard]]` Usage**: Applied across the codebase to prevent silent logic errors.
- **Boost-style Hash Combining**: Optimized the `HexCoordHash` for the sparse chunk system.

## 4. Emergent Complexity
In verification runs, the engine successfully generated over **90 unique species** within 200 ticks. The lineage tree shows deep branching, where sexual crossover allowed successful traits (like armor and high-efficiency mouths) to spread across different lineages.

---
### Technical Summary
- **Crossover Method**: Single-point split for both Anatomy and Neural Synapses.
- **Disaster Logic**: Deterministic, duration-based state machine integrated into the `Simulator`.
- **Performance**: Maintained high tick rates even with 70+ active organisms and 60+ extant species.
