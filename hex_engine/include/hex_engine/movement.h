#pragma once

#include <cstdint>
#include <optional>

#include "hex_engine/hex_coord.h"
#include "hex_engine/world.h"

namespace hex_engine {

// Movement attempt result.
enum class MoveResult {
    Success,           // Cell moved successfully.
    Blocked,           // Target cell is occupied by a wall or non-empty cell.
    OutOfBounds,       // Target is outside the world (if bounded).
    InsufficientEnergy, // Cell doesn't have enough energy to move.
};

// Movement system handles organism locomotion and direction changes.
class MovementSystem {
public:
    MovementSystem() = default;
    
    // Try to move a cell in a given direction.
    // Returns the result and optionally the new coordinate if successful.
    [[nodiscard]] MoveResult try_move_cell(World& world, HexCoord from, HexDirection direction,
                                          float energy_cost);
    
    // Rotate an organism's facing direction (for future AI/behavior).
    void rotate_organism(World& world, const std::vector<HexCoord>& organism_cells,
                        HexDirection new_facing);
    
    // Check if a target coordinate is passable (empty or food).
    [[nodiscard]] bool is_passable(const World& world, HexCoord target) const noexcept;
    
    // Get the next direction in a rotation (clockwise).
    [[nodiscard]] static constexpr HexDirection rotate_clockwise(HexDirection dir) noexcept {
        return static_cast<HexDirection>((static_cast<std::uint8_t>(dir) + 1) % 6);
    }
    
    // Get the next direction counter-clockwise.
    [[nodiscard]] static constexpr HexDirection rotate_counter_clockwise(HexDirection dir) noexcept {
        const auto val = static_cast<std::uint8_t>(dir);
        return static_cast<HexDirection>(val == 0 ? 5 : val - 1);
    }
};

} // namespace hex_engine
