#pragma once

#include <cstddef>
#include <cstdint>
#include <unordered_map>
#include <vector>
#include <memory>

#include "hex_engine/cell_kind.h"
#include "hex_engine/hex_coord.h"

namespace hex_engine {

struct Cell final {
    CellKind kind = CellKind::Empty;
    std::uint32_t age = 0;
    float energy = 1.0f;
    HexDirection facing = HexDirection::East;
};

struct Pheromones final {
    float values[3] = {0.0f, 0.0f, 0.0f};
};

// A Chunk represents a fixed-size region of the world for optimization.
struct Chunk final {
    static constexpr int kSize = 16; // Chunk radius
    std::unordered_map<HexCoord, Cell, HexCoordHash> cells;
    std::unordered_map<HexCoord, Pheromones, HexCoordHash> pheromones;
};

class World final {
public:
    using ChunkMap = std::unordered_map<HexCoord, std::unique_ptr<Chunk>, HexCoordHash>;

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
    void diffuse_pheromones(float diffusion_rate);
    
    // Environment
    [[nodiscard]] float light_at(const HexCoord coord, float gradient_factor) const noexcept;
    [[nodiscard]] uint32_t total_ticks() const noexcept { return total_ticks_; }
    void increment_ticks() noexcept { total_ticks_++; }

    // Deterministic ordering
    [[nodiscard]] std::vector<HexCoord> occupied_coords() const;
    [[nodiscard]] std::vector<HexCoord> pheromone_coords() const;

private:
    ChunkMap chunks_;
    std::size_t total_cells_ = 0;
    uint32_t total_ticks_ = 0;

    Chunk& get_or_create_chunk(const HexCoord coord);
    [[nodiscard]] HexCoord world_to_chunk_coord(const HexCoord coord) const noexcept;
};

struct WorldBounds final {
    HexCoord min{};
    HexCoord max{};
    bool valid = false;
};

[[nodiscard]] WorldBounds compute_bounds(const World& world);

} // namespace hex_engine
