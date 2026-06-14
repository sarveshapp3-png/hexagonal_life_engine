#pragma once

#include <cstddef>
#include <cstdint>
#include <unordered_map>
#include <vector>

#include "hex_engine/cell_kind.h"
#include "hex_engine/hex_coord.h"

namespace hex_engine {

// A cell is a small bit of state attached to a coordinate. For the first
// milestone we only need the kind and a simple age counter.
struct Cell final {
    CellKind kind = CellKind::Empty;   // Cell role or state.
    std::uint32_t age = 0;             // Tick age for future lifecycle rules.
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
    [[nodiscard]] CellKind kind_at(const HexCoord coord) const noexcept;

    void set_cell(const HexCoord coord, const CellKind kind);
    void clear_cell(const HexCoord coord);

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

