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
    Sense,       // Internal state sensor (Energy, Age).
    Signal,      // Pheromone detector/emitter.
    Virus,       // Injects genome into neighbors.
    Scavenger,   // Harvests energy from dead cells.
    Explosive,   // High damage, kills self.
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
    case CellKind::Sense:
        return "Sense";
    case CellKind::Signal:
        return "Signal";
    case CellKind::Virus:
        return "Virus";
    case CellKind::Scavenger:
        return "Scavenger";
    case CellKind::Explosive:
        return "Explosive";
    }

    // Returning a fallback keeps the function total even if we add new enum
    // values later and forget to update this switch immediately.
    return "Unknown";
}

[[nodiscard]] constexpr CellKind cell_kind_from_name(const std::string_view name) noexcept {
    if (name == "Food") return CellKind::Food;
    if (name == "Wall") return CellKind::Wall;
    if (name == "Mouth") return CellKind::Mouth;
    if (name == "Producer") return CellKind::Producer;
    if (name == "Mover") return CellKind::Mover;
    if (name == "Killer") return CellKind::Killer;
    if (name == "Armor") return CellKind::Armor;
    if (name == "Eye") return CellKind::Eye;
    if (name == "Sense") return CellKind::Sense;
    if (name == "Signal") return CellKind::Signal;
    if (name == "Virus") return CellKind::Virus;
    if (name == "Scavenger") return CellKind::Scavenger;
    if (name == "Explosive") return CellKind::Explosive;
    return CellKind::Empty;
}

} // namespace hex_engine

