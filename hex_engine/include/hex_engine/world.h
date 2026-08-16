#pragma once

#include <cstddef>
#include <cstdint>
#include <unordered_map>
#include <vector>

#include "hex_engine/cell_kind.h"
#include "hex_engine/hex_coord.h"

namespace hex_engine {

// A cell is a small bit of state attached to a coordinate.
struct Cell final {
    CellKind kind = CellKind::Empty;   // Cell role or state.
    std::uint32_t age = 0;             // Tick age for lifecycle rules.
    float energy = 1.0f;               // Energy level for metabolism.
    HexDirection facing = HexDirection::East;  // Orientation for movement.
};

// Pheromones are values stored on the grid that decay and diffuse.
struct Pheromones final {
    float values[3] = {0.0f, 0.0f, 0.0f}; // Support for multiple pheromone types.
};

// Sparse storage for cells and pheromones.
class World final {
public:
    using CellMap = std::unordered_map<HexCoord, Cell, HexCoordHash>;
    using PheromoneMap = std::unordered_map<HexCoord, Pheromones, HexCoordHash>;

    void clear();

    // Cell operations
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

    // Pheromone operations
    [[nodiscard]] float pheromone_at(const HexCoord coord, int type) const noexcept;
    void add_pheromone(const HexCoord coord, int type, float amount);
    void decay_pheromones(float decay_factor);
    
    // Deterministic ordering
    [[nodiscard]] std::vector<HexCoord> occupied_coords() const;
    [[nodiscard]] std::vector<HexCoord> pheromone_coords() const;

private:
    CellMap cells_;
    PheromoneMap pheromones_;
};

struct WorldBounds final {
    HexCoord min{};
    HexCoord max{};
    bool valid = false;
};

[[nodiscard]] WorldBounds compute_bounds(const World& world);

} // namespace hex_engine
