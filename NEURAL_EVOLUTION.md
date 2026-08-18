# Hexagonal Life Engine: Neural Evolution & Environmental Dynamics

This update introduces a major leap in complexity, moving from simple reaction maps to a full **Neural Network Architecture** and integrated **Environmental Gradients**, bringing the engine into close alignment with advanced Life Engine mods.

## 1. The Neural Brain Architecture
Organisms now possess a feed-forward **Neural Network** that processes sensory data and drives complex emergent behaviors.

### Sensory Inputs (8 Channels)
- **Vision (0-1)**: Normalized `CellKind` and `Distance` detected by `Eye` cells.
- **Internal State (2-3)**: Normalized `Energy` and `Age` levels detected by `Sense` cells.
- **Environment (4)**: Local `Light` intensity detected by `Sense` cells.
- **Signaling (5-7)**: Three channels of `Pheromone` detection via `Signal` cells.

### Motor Outputs (5 Channels)
- **Locomotion**: `Move Forward` command.
- **Rotation**: `Rotate Left` and `Rotate Right` commands.
- **Life Cycle**: `Reproduce` command (requires energy threshold).
- **Communication**: `Emit Pheromone` command.

## 2. Sensory Gating & Specialization
To make evolution more meaningful, the brain's inputs are now **gated by anatomy**. An organism only receives vision data if it has evolved `Eye` cells, and only knows its own energy/age if it has `Sense` cells. This creates a selective pressure for organisms to evolve specialized sensory organs.

## 3. Environmental Dynamics
The world is no longer a static void. It now features dynamic gradients that challenge survival:
- **Light Gradient**: Light intensity decreases with distance from the equator (R=0). `Producer` cells generate energy proportional to local light.
- **Temperature Stress**: A temperature gradient creates "ideal" zones. Moving too far from these zones increases the **Base Energy Decay**, simulating environmental stress.

## 4. Internal Signaling & Pheromones
Organisms can now communicate across space and time:
- **Signal Cells**: Act as both detectors and emitters of pheromones.
- **Pheromone Grid**: Signals diffuse through the hex grid and decay over time, allowing for the evolution of social behaviors like swarming or territory marking.

## 5. Parity with Advanced Mods
This implementation achieves parity with "Neural Brain" mods by replacing static lookups with evolvable **Synapses**. The `Genome::distance` function now accounts for neural architecture, allowing the `FossilRecord` to track deep evolutionary branching.

---
### Technical Summary
- **Activation Function**: `std::tanh` for all neural nodes.
- **Mutation Model**: Evolvable synapse weights, addition/removal of connections.
- **Deterministic**: Perfectly reproducible neural processing across ticks.
