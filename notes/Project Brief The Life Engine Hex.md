# Project Brief: The Life Engine Hex

## Vault Map

- [[Overview]]
- [[Architecture/Hex Engine Architecture]]
- [[Systems/Hex Simulation Core]]
- [[Features/Initial Scaffold]]
- [[Decisions/Foundation Decisions]]
- [[Roadmap/Hex Engine Roadmap]]
- [[Branding/Life Engine Logo]]
- [[Checklist/Laptop And Project Setup]]

## 1. Vision & Overview

**The Life Engine Hex** is a high-fidelity, mobile-optimized evolution simulator. It is a direct "hexagonal translation" of the original *thelifeengine.net*. The project replaces the traditional square grid with a dense hexagonal coordinate system, requiring a complete re-engineering of organism structures, movement logic, and genetic interactions while maintaining the "technical utility" aesthetic of the source material.

Current implementation starts with the smallest possible foundation in [[Features/Initial Scaffold]] so the project can grow one seam at a time.

## 2. Core Requirements

### 2.1 Hexagonal Simulation Engine

- **Grid Geometry**: Transition from Cartesian (square) to Axial or Offset hexagonal coordinates.
- **Organism Logic**: Multi-cellular organisms must form contiguous clusters of hexagonal cells.
- **Movement**: Organisms navigate in 6 directions (60-degree increments) rather than 4 or 8.
- **Cell Types**: Maintain original functional logic for:
  - **Mover (Green)**: Provides kinetic energy and propulsion.
  - **Producer (Cyan)**: Photosynthetic organelle generating energy.
  - **Eater (Orange/Peach)**: Harvests organic material from other cells.
  - **Shield (Yellow)**: Defensive exoskeleton reducing damage.
  - **Brain (Gold)**: Neural processing unit for complex behavior.
  - **Spore (Pink)**: Reproductive emitter for species propagation.

### 2.2 User Interface (UI) & Experience (UX)

- **Pixel-Perfect Replication**: UI overlays (Organism Details, Evolution Controls, Statistics) must mirror the layout, hierarchy, and "no-frills" utility of the source site.
- **Mobile Optimization**: High-density desktop controls are adapted into collapsible bottom sheets and drawers to maximize simulation visibility on mobile screens.
- **Real-time Overlays**: Floating "Organism Details" and "Target FPS" counters that track live simulation data.

## 3. Screen Specifications

### 3.1 Simulation Viewer (Main)

- **Primary Surface**: Infinite (or wrap-around) hexagonal grid.
- **Controls**: Speed sliders, pause/resume, reset, and world-saving options.
- **Evolution Panel**: Granular sliders for mutation rates, food production probability, and lifespan multipliers.

### 3.2 Genome Editor

- **Drawing Grid**: Dedicated hexagonal canvas for specimen design.
- **Library**: Quick-access cell palette with technical spec readouts (Cell Count, Energy Index).
- **Deployment**: One-tap injection of custom species into the active simulation.

### 3.3 Ecosystem Stats

- **Data Tracking**: Population growth charts (Live Ticks), species diversity metrics, and mutation deltas.
- **Functional Distribution**: Radial or bar charts showing the metabolic balance of the current ecosystem (Efficient vs. Passive).

### 3.4 Community Hub

- **Repository**: Shared "World Seeds," "Species Files," and "Expansion Modules."
- **Social Integration**: Ability to share creations and import community-evolved organisms.

## 4. Design System: "Synthetic Lab"

- **Palette**:
  - Surface: `#081425` (Deep Navy)
  - Primary: `#22c55e` (Neon Green)
  - Accents: Neon Cyan, Mutation Pink, Warning Yellow.
- **Typography**: `JetBrains Mono` or `Hanken Grotesk` for a technical, data-driven feel.
- **Roundness**: `4px` (Minimalist, industrial feel).

## 5. Technical Constraints

- **Performance**: Must maintain a steady 60 FPS on mobile devices despite high cell counts.
- **Coordinate Mapping**: Ensure the hex-grid math (neighbors, distance, line-of-sight) is consistent across all screens (Viewer vs. Editor).
- **Persistence**: Local storage or JSON export/import for world states and species genomes.
