#pragma once

#include <cstddef>
#include <cstdint>
#include <unordered_map>
#include <vector>

#include "hex_engine/cell_kind.h"
#include "hex_engine/hex_coord.h"

namespace hex_engine {

// A cell is a small bit of state attached to a coordinate. We track kind, age,
// energy for metabolism, and direction for movement logic.
struct Cell final {
    CellKind kind = CellKind::Empty;   // Cell role or state.
    std::uint32_t age = 0;             // Tick age for lifecycle rules.
    float energy = 1.0f;               // Energy level for metabolism.
    HexDirection facing = HexDirection::East;  // Orientation for movement.
};

// Sparse storage is the safest default because the world may eventually become
// very large while remaining mostly empty.
class World final {
public:
    using CellMap = std::unordered_map<HexCoord, Cell, HexCoordHash>;

    void clear();

    [[nodiscard]] bool empty() const noexcept;
    [[nodiscard]] std::size_t occupied_count() const noexcept;
    [[nodiscard]] bool contains(const HexCoord coord) const noexcept;
    [[nodiscard]] const Cell* find_cell(const HexCoord coord) const noexcept;
    [[nodiscard]] Cell* find_cell_mut(const HexCoord coord) noexcept;
    [[nodiscard]] CellKind kind_at(const HexCoord coord) const noexcept;
    [[nodiscard]] float energy_at(const HexCoord coord) const noexcept;

    void set_cell(const HexCoord coord, const CellKind kind);
    void set_cell_with_energy(const HexCoord coord, const CellKind kind, float energy);
    void set_cell_full(const HexCoord coord, const Cell& cell);
    void clear_cell(const HexCoord coord);
    void add_energy(const HexCoord coord, float amount);
    void set_facing(const HexCoord coord, HexDirection direction);

    // Deterministic ordering matters for logs, tests, replays, and save files.
    [[nodiscard]] std::vector<HexCoord> occupied_coords() const;

private:
    CellMap cells_; // Sparse cell storage keyed by axial hex coordinate.
};

// Bounds are a useful cheap summary for future camera fitting and chunk sizing.
struct WorldBounds final {
    HexCoord min{}; // Inclusive minimum coordinate.
    HexCoord max{}; // Inclusive maximum coordinate.
    bool valid = false; // False means the world is empty.
};

[[nodiscard]] WorldBounds compute_bounds(const World& world);

} // namespace hex_engine
