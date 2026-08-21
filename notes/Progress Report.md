# Hexagonal Life Engine: Progress Report

The **Hexagonal Life Engine** is a high-performance, deterministic simulation environment designed to explore emergent multi-cellular evolution. This project adapts the core mechanics of the original [thelifeengine.net](https://thelifeengine.net) to a hexagonal grid, providing more natural movement and neighborhood relationships while maintaining strict parity with the original's biological logic.

## Core Architectural Foundations

The simulation operates on an **Axial Coordinate System**, which enables efficient hex math for distance, rotation, and line-of-sight calculations. By utilizing a **Sparse World Storage** model based on `std::unordered_map`, the engine can simulate massive environments without the memory overhead of a dense grid. Every simulation step is processed in a **Deterministic Tick Loop**, ensuring that the evolution of species is perfectly reproducible across different platforms and sessions.

## Multi-cellular Organism Dynamics

Organisms in the engine are more than just clusters of cells; they are defined by a **Genome-based Anatomy**. This genome acts as a template, specifying the relative hex positions and roles of every cell within the organism. A critical feature of this implementation is the **Unified Energy Pool**, where every cell in an organism contributes its harvested energy to a shared resource, which is then used for metabolism, movement, and reproduction.

### Cell Kind Specializations

To match the functional depth of the original Life Engine, the following cell types have been implemented with hexagonal logic:

| Cell Kind | Functional Description |
|-----------|------------------------|
| **Producer** | Synthesizes energy for the organism, simulating photosynthesis. |
| **Mouth** | Actively harvests energy from adjacent food sources. |
| **Mover** | Provides kinetic force, allowing the organism to navigate the grid. |
| **Killer** | An offensive cell that damages adjacent organisms. |
| **Armor** | A defensive layer that negates incoming damage from killers. |
| **Eye** | Performs raycasting in the organism's facing direction to detect objects. |
| **Brain** | A decision-making unit that maps vision data to behavioral reactions. |
| **Sense** | Detects internal state (Energy, Age) and environmental light. |
| **Signal** | Detects and emits pheromones for communication. |
| **Virus** | Injects its genome into neighboring organisms. |
| **Scavenger** | Specialized harvester for dead cell energy. |
| **Explosive** | Detonates when health is low, damaging a large area. |
| **Wall** | Indestructible blocker that restricts movement. |
| **Poison** | Lethal hazard that damages organisms when eaten. |

## Evolution and Environmental Emergence
The ecosystem has reached a peak level of complexity with the introduction of **Sexual Reproduction (Gene Crossover)**. Organisms can now exchange genetic material with mates, leading to rapid trait recombination and hybrid species. This is complemented by a **Dynamic Disaster System** (Radiation, Blight, Solar Flares) that periodically shifts the selective pressures of the world.

The environment also features **Global Food Spawning**, creating a dynamic resource landscape that encourages nomadic behavior. To counter this, **Walls** and **Poison** have been introduced, forcing organisms to evolve spatial awareness and sensory discrimination. The engine supports deep **Lineage Tracking**, allowing us to trace the ancestry of every species back to its founding genome through directed family trees.

Furthermore, the introduction of **Parasitic (Virus)** and **Decomposer (Scavenger)** roles has significantly increased the emergent complexity of the ecosystem, leading to predator-prey-parasite cycles. Organisms also suffer from **Temperature Stress** when venturing too far from ideal zones, creating a dynamic map where location matters for survival.
The engine now features a **Neural Network Brain** that replaces simple reaction maps. Organisms evolve **Synapses** that connect sensory inputs (Vision, Sense, Signal) to motor outputs (Move, Rotate, Reproduce). This allows for the emergence of complex, multi-step behaviors.

Furthermore, the environment now includes **Light and Temperature Gradients**. Producers are light-dependent, and organisms suffer from **Temperature Stress** when venturing too far from ideal zones, creating a dynamic map where location matters for survival.

The engine facilitates true **Natural Selection** through a robust mutation system. During reproduction, offspring have a probability of undergoing **Anatomy Mutation** (adding, removing, or changing cells) and **Brain Mutation** (altering reactions to seen objects). This leads to the emergence of specialized species, such as efficient producers, agile predators, and armored prey.

Beyond cellular logic, the environment supports a **Pheromone Grid** where chemical signals diffuse and decay over time, allowing for advanced emergent behaviors like trail-following or territory marking. The simulation's parameters are fully configurable via **Lua Scripting**, and the entire world state can be persisted through a **JSON Save/Load System**.

## Implementation Standards

This project adheres to modern C++20 standards, emphasizing memory safety through **RAII and Smart Pointers**. The codebase is structured with a clear **Separation of Concerns**, decoupling low-level grid math from high-level biological simulation. All progress is synchronized with GitHub to ensure transparency and collaborative potential.

---
### References
- [1] [The Life Engine Official Website](https://thelifeengine.net)
- [2] [GitHub Repository: Hexagonal Life Engine](https://github.com/sarveshapp3-png/hexagonal_life_engine)
