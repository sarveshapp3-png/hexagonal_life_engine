# Phase 2 Implementation Summary

## Overview

Phase 2 adds deterministic tick processing, energy metabolism, organism clustering, and basic movement logic to the hexagonal life engine. The foundation is now ready for reproduction, vision, and advanced AI behaviors.

## What Was Implemented

### 1. Simulator Class (`simulation.h` / `simulation.cpp`)

The `Simulator` wraps the `World` and orchestrates tick-based processing:

- **`tick()`**: Main simulation loop that processes all behaviors in deterministic order
- **`process_producers()`**: Producers generate energy (+0.5/tick by default)
- **`process_mouths()`**: Mouths harvest energy from adjacent food cells (80% efficiency)
- **`process_movers()`**: Movers consume energy for movement (-0.3/tick)
- **`process_movement()`**: Movers attempt to move in their facing direction
- **`apply_energy_decay()`**: All cells lose 10% of energy per tick; cells starve at 0 energy

**Configuration**: `SimulationConfig` allows tuning of all parameters via Lua or code.

### 2. Cell State Expansion

The `Cell` struct now includes:

```cpp
struct Cell {
    CellKind kind;              // Cell role (Producer, Mover, etc.)
    std::uint32_t age;          // Tick age for lifecycle rules
    float energy;               // Energy level for metabolism
    HexDirection facing;        // Orientation for movement
};
```

New `World` methods:
- `find_cell_mut()`: Mutable access to cells for state updates
- `energy_at()`: Query energy level
- `add_energy()`: Modify energy (with bounds checking)
- `set_facing()`: Update cell orientation
- `set_cell_with_energy()`: Create cells with specific energy

### 3. Organism Clustering (`organism.h` / `organism.cpp`)

`OrganismRegistry` tracks contiguous cell clusters:

- **BFS-based clustering**: Cells of the same kind connected via hex neighbors form one organism
- **Organism statistics**: Total energy, cell count, age, unique ID
- **Fast lookup**: `find_organism_at()` and `find_organism_by_id()` for quick queries

Example output:
```
Organism #1: 3 Mover cells, 15 total energy
Organism #2: 1 Food cell, 10 energy
Organism #3: 2 Producer cells, 6 total energy
```

### 4. Movement System (`movement.h` / `movement.cpp`)

`MovementSystem` handles organism locomotion:

- **`try_move_cell()`**: Move a cell in a direction if passable and energy sufficient
  - Returns `MoveResult`: Success, Blocked, OutOfBounds, InsufficientEnergy
  - Empty cells and food are passable; walls block movement
  - Movement costs energy and increments cell age
  
- **`rotate_organism()`**: Update facing direction for all cells in an organism
- **`is_passable()`**: Check if target is empty or food
- **Rotation helpers**: `rotate_clockwise()` and `rotate_counter_clockwise()`

### 5. Demo Updates

The demo now shows:

1. **Initial state**: 8 cells seeded with specific energy levels
2. **Organism clustering**: 5 organisms detected (3 Movers, 2 Producers, 1 Food, 1 Mouth, 1 Wall)
3. **3 ticks of simulation**:
   - Producers gain energy (+0.5/tick)
   - Mouths harvest food (Food: 10 → 1.8 → 0.32 → removed)
   - All cells decay energy (10%/tick)
   - Cells starve and disappear when energy reaches 0

## Architecture

```
World (sparse hex grid)
  ├─ Cell storage (HexCoord → Cell)
  └─ Bounds computation

Simulator (tick orchestration)
  ├─ process_producers()
  ├─ process_mouths()
  ├─ process_movers()
  ├─ process_movement()
  └─ apply_energy_decay()

OrganismRegistry (clustering)
  └─ BFS clustering for contiguous cells

MovementSystem (locomotion)
  └─ Passability checks & movement validation
```

## Key Design Decisions

1. **Deterministic ordering**: Producers → Mouths → Movers → Movement → Decay
   - Ensures reproducible simulations and replays
   
2. **Sparse storage**: Only occupied cells stored; empty cells implicit
   - Scales to very large worlds with few organisms
   
3. **Energy-based metabolism**: All behaviors consume or generate energy
   - Starvation naturally removes weak organisms
   - Creates pressure for efficient designs
   
4. **Organism clustering by kind**: Same-kind cells form one organism
   - Simplifies multi-cellular logic
   - Future: support mixed-kind organisms with neural networks
   
5. **Passable terrain**: Empty + Food allow movement
   - Walls and other organisms block
   - Food can be consumed during movement (future)

## Testing

Build and run:
```bash
cd hex_engine/build
cmake ..
make
./hex_engine_demo
```

Expected output: 3 ticks showing energy flow, clustering, and starvation mechanics.

## Next Steps (Phase 3+)

1. **Reproduction**: Organisms spawn new cells when energy exceeds threshold
2. **Mutation**: Genetic variation in cell types and behaviors
3. **Vision**: Eye cells feed data to future brain logic
4. **Brain cells**: Neural networks for decision-making
5. **Rendering**: OpenGL visualization of hex world
6. **Lua integration**: Data-driven rules and presets
7. **Persistence**: Save/load world state
8. **Editor**: Genome design and world creation tools

## Performance Notes

- Current demo: 8 cells, instant tick processing
- Sparse storage scales well: 1M cells with 1% occupancy = ~10K actual cells
- BFS clustering: O(n) where n = number of cells
- Movement: O(n) per tick for Movers
- No optimization needed until reaching 10K+ cells at 60 FPS

## Files Changed

- `include/hex_engine/simulation.h` (NEW)
- `include/hex_engine/organism.h` (NEW)
- `include/hex_engine/movement.h` (NEW)
- `include/hex_engine/world.h` (UPDATED: added energy, facing, new methods)
- `src/world.cpp` (UPDATED: new cell state methods)
- `src/simulation.cpp` (NEW)
- `src/organism.cpp` (NEW)
- `src/movement.cpp` (NEW)
- `src/main.cpp` (UPDATED: demo with clustering and movement)
- `CMakeLists.txt` (UPDATED: added new source files)
