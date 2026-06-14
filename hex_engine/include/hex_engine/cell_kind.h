#pragma once

#include <cstdint>
#include <string_view>

namespace hex_engine {

// We keep the original Life Engine roles so the design can stay faithful while
// the geometry underneath changes from square to hex.
enum class CellKind : std::uint8_t {
    Empty = 0,   // No cell stored in the world.
    Food,        // Passive resource cell.
    Wall,        // Solid blocker for movement and placement.
    Mouth,       // Consumer cell that will later harvest food.
    Producer,    // Resource-generating cell.
    Mover,       // Movement and locomotion cell.
    Killer,      // Offensive cell that damages neighbors.
    Armor,       // Defensive cell that negates damage.
    Eye,         // Vision cell that will feed the future brain logic.
};

// Human-readable labels are useful immediately for debug output and later for
// the UI, save files, and scripting bridges.
[[nodiscard]] constexpr std::string_view cell_kind_name(const CellKind kind) noexcept {
    switch (kind) {
    case CellKind::Empty:
        return "Empty";
    case CellKind::Food:
        return "Food";
    case CellKind::Wall:
        return "Wall";
    case CellKind::Mouth:
        return "Mouth";
    case CellKind::Producer:
        return "Producer";
    case CellKind::Mover:
        return "Mover";
    case CellKind::Killer:
        return "Killer";
    case CellKind::Armor:
        return "Armor";
    case CellKind::Eye:
        return "Eye";
    }

    // Returning a fallback keeps the function total even if we add new enum
    // values later and forget to update this switch immediately.
    return "Unknown";
}

} // namespace hex_engine

