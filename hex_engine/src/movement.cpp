#include "hex_engine/movement.h"

namespace hex_engine {

MoveResult MovementSystem::try_move_cell(World& world, HexCoord from, HexDirection direction,
                                        float energy_cost) {
    // Check if the source cell has enough energy.
    float current_energy = world.energy_at(from);
    if (current_energy < energy_cost) {
        return MoveResult::InsufficientEnergy;
    }
    
    // Calculate the target coordinate.
    const HexCoord target = hex_neighbor(from, direction);
    
    // Check if the target is passable.
    if (!is_passable(world, target)) {
        return MoveResult::Blocked;
    }
    
    // Get the cell being moved.
    const Cell* source_cell = world.find_cell(from);
    if (source_cell == nullptr) {
        return MoveResult::Blocked;
    }
    
    // Perform the move: copy cell to target and remove from source.
    Cell moving_cell = *source_cell;
    moving_cell.energy -= energy_cost;
    moving_cell.age += 1;
    
    world.set_cell(target, moving_cell.kind);
    
    // Transfer energy to the target cell.
    world.add_energy(target, moving_cell.energy);
    world.set_facing(target, direction);
    
    // Clear the source cell.
    world.clear_cell(from);
    
    return MoveResult::Success;
}

void MovementSystem::rotate_organism(World& world, const std::vector<HexCoord>& organism_cells,
                                    HexDirection new_facing) {
    // Update the facing direction for all cells in the organism.
    for (const HexCoord coord : organism_cells) {
        world.set_facing(coord, new_facing);
    }
}

bool MovementSystem::is_passable(const World& world, HexCoord target) const noexcept {
    const CellKind kind = world.kind_at(target);
    
    // Empty cells and food are passable.
    // Walls, other organisms, and non-passable types block movement.
    return kind == CellKind::Empty || kind == CellKind::Food;
}

} // namespace hex_engine
