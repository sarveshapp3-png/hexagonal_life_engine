#include "hex_engine/world.h"
#include <algorithm>
#include <cmath>

namespace hex_engine {

void World::clear() {
    chunks_.clear();
    total_cells_ = 0;
}

bool World::empty() const noexcept {
    return total_cells_ == 0;
}

std::size_t World::occupied_count() const noexcept {
    return total_cells_;
}

HexCoord World::world_to_chunk_coord(const HexCoord coord) const noexcept {
    // Basic tiling for hex chunks. 
    // In a real hex tiling, this is more complex, but simple rectangular axial tiling works for sparse lookups.
    return { 
        static_cast<int>(std::floor(static_cast<float>(coord.q) / Chunk::kSize)),
        static_cast<int>(std::floor(static_cast<float>(coord.r) / Chunk::kSize))
    };
}

Chunk& World::get_or_create_chunk(const HexCoord coord) {
    HexCoord cc = world_to_chunk_coord(coord);
    auto it = chunks_.find(cc);
    if (it == chunks_.end()) {
        chunks_[cc] = std::make_unique<Chunk>();
        return *chunks_[cc];
    }
    return *it->second;
}

bool World::contains(const HexCoord coord) const noexcept {
    HexCoord cc = world_to_chunk_coord(coord);
    auto it = chunks_.find(cc);
    if (it == chunks_.end()) return false;
    return it->second->cells.find(coord) != it->second->cells.end();
}

const Cell* World::find_cell(const HexCoord coord) const noexcept {
    HexCoord cc = world_to_chunk_coord(coord);
    auto it = chunks_.find(cc);
    if (it == chunks_.end()) return nullptr;
    auto cell_it = it->second->cells.find(coord);
    if (cell_it == it->second->cells.end()) return nullptr;
    return &cell_it->second;
}

Cell* World::find_cell_mut(const HexCoord coord) noexcept {
    HexCoord cc = world_to_chunk_coord(coord);
    auto it = chunks_.find(cc);
    if (it == chunks_.end()) return nullptr;
    auto cell_it = it->second->cells.find(coord);
    if (cell_it == it->second->cells.end()) return nullptr;
    return &cell_it->second;
}

CellKind World::kind_at(const HexCoord coord) const noexcept {
    const Cell* cell = find_cell(coord);
    return cell ? cell->kind : CellKind::Empty;
}

float World::energy_at(const HexCoord coord) const noexcept {
    const Cell* cell = find_cell(coord);
    return cell ? cell->energy : 0.0f;
}

void World::set_cell(const HexCoord coord, const CellKind kind) {
    if (kind == CellKind::Empty) {
        clear_cell(coord);
        return;
    }
    Chunk& chunk = get_or_create_chunk(coord);
    if (chunk.cells.find(coord) == chunk.cells.end()) total_cells_++;
    chunk.cells[coord] = Cell{kind, 0U, 1.0f, HexDirection::East};
}

void World::set_cell_with_energy(const HexCoord coord, const CellKind kind, float energy) {
    if (kind == CellKind::Empty) {
        clear_cell(coord);
        return;
    }
    Chunk& chunk = get_or_create_chunk(coord);
    if (chunk.cells.find(coord) == chunk.cells.end()) total_cells_++;
    chunk.cells[coord] = Cell{kind, 0U, energy, HexDirection::East};
}

void World::set_cell_full(const HexCoord coord, const Cell& cell) {
    if (cell.kind == CellKind::Empty) {
        clear_cell(coord);
        return;
    }
    Chunk& chunk = get_or_create_chunk(coord);
    if (chunk.cells.find(coord) == chunk.cells.end()) total_cells_++;
    chunk.cells[coord] = cell;
}

void World::clear_cell(const HexCoord coord) {
    HexCoord cc = world_to_chunk_coord(coord);
    auto it = chunks_.find(cc);
    if (it != chunks_.end()) {
        if (it->second->cells.erase(coord) > 0) total_cells_--;
    }
}

void World::add_energy(const HexCoord coord, float amount) {
    Cell* cell = find_cell_mut(coord);
    if (cell) {
        cell->energy += amount;
        if (cell->energy > 100.0f) cell->energy = 100.0f;
        if (cell->energy < 0.0f) cell->energy = 0.0f;
    }
}

void World::set_facing(const HexCoord coord, HexDirection direction) {
    Cell* cell = find_cell_mut(coord);
    if (cell) cell->facing = direction;
}

float World::pheromone_at(const HexCoord coord, int type) const noexcept {
    if (type < 0 || type >= 3) return 0.0f;
    HexCoord cc = world_to_chunk_coord(coord);
    auto it = chunks_.find(cc);
    if (it == chunks_.end()) return 0.0f;
    auto p_it = it->second->pheromones.find(coord);
    if (p_it == it->second->pheromones.end()) return 0.0f;
    return p_it->second.values[type];
}

void World::add_pheromone(const HexCoord coord, int type, float amount) {
    if (type < 0 || type >= 3) return;
    Chunk& chunk = get_or_create_chunk(coord);
    chunk.pheromones[coord].values[type] += amount;
    if (chunk.pheromones[coord].values[type] > 10.0f) chunk.pheromones[coord].values[type] = 10.0f;
}

void World::decay_pheromones(float decay_factor) {
    for (auto& [cc, chunk] : chunks_) {
        for (auto it = chunk->pheromones.begin(); it != chunk->pheromones.end(); ) {
            bool all_zero = true;
            for (int i = 0; i < 3; ++i) {
                it->second.values[i] *= decay_factor;
                if (it->second.values[i] < 0.01f) it->second.values[i] = 0.0f;
                else all_zero = false;
            }
            if (all_zero) it = chunk->pheromones.erase(it);
            else ++it;
        }
    }
}

void World::diffuse_pheromones(float diffusion_rate) {
    // Simple diffusion: for each pheromone, spread to neighbors.
    // In chunked world, we collect updates first to maintain determinism.
    struct Update { HexCoord coord; int type; float amount; };
    std::vector<Update> updates;

    for (auto& [cc, chunk] : chunks_) {
        for (auto& [coord, p] : chunk->pheromones) {
            for (int i = 0; i < 3; ++i) {
                float spread = p.values[i] * diffusion_rate / 6.0f;
                if (spread < 0.001f) continue;
                for (int dir = 0; dir < 6; ++dir) {
                    updates.push_back({hex_neighbor(coord, static_cast<HexDirection>(dir)), i, spread});
                    updates.push_back({coord, i, -spread});
                }
            }
        }
    }
    for (const auto& up : updates) add_pheromone(up.coord, up.type, up.amount);
}

std::vector<HexCoord> World::occupied_coords() const {
    std::vector<HexCoord> coords;
    coords.reserve(total_cells_);
    for (auto& [cc, chunk] : chunks_) {
        for (auto& [coord, cell] : chunk->cells) coords.push_back(coord);
    }
    std::sort(coords.begin(), coords.end(), [](const HexCoord l, const HexCoord r) {
        if (l.q != r.q) return l.q < r.q;
        return l.r < r.r;
    });
    return coords;
}

std::vector<HexCoord> World::pheromone_coords() const {
    std::vector<HexCoord> coords;
    for (auto& [cc, chunk] : chunks_) {
        for (auto& [coord, p] : chunk->pheromones) coords.push_back(coord);
    }
    std::sort(coords.begin(), coords.end(), [](const HexCoord l, const HexCoord r) {
        if (l.q != r.q) return l.q < r.q;
        return l.r < r.r;
    });
    return coords;
}

WorldBounds compute_bounds(const World& world) {
    WorldBounds bounds;
    const std::vector<HexCoord> coords = world.occupied_coords();
    if (coords.empty()) return bounds;
    bounds.valid = true;
    bounds.min = coords.front();
    bounds.max = coords.front();
    for (const HexCoord coord : coords) {
        bounds.min.q = std::min(bounds.min.q, coord.q);
        bounds.min.r = std::min(bounds.min.r, coord.r);
        bounds.max.q = std::max(bounds.max.q, coord.q);
        bounds.max.r = std::max(bounds.max.r, coord.r);
    }
    return bounds;
}

} // namespace hex_engine
