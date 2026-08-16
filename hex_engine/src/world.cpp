#include "hex_engine/world.h"

#include <algorithm>

namespace hex_engine {

void World::clear() {
    cells_.clear();
}

bool World::empty() const noexcept {
    return cells_.empty();
}

std::size_t World::occupied_count() const noexcept {
    return cells_.size();
}

bool World::contains(const HexCoord coord) const noexcept {
    return cells_.find(coord) != cells_.end();
}

const Cell* World::find_cell(const HexCoord coord) const noexcept {
    const auto iter = cells_.find(coord);
    if (iter == cells_.end()) {
        return nullptr;
    }
    return &iter->second;
}

Cell* World::find_cell_mut(const HexCoord coord) noexcept {
    const auto iter = cells_.find(coord);
    if (iter == cells_.end()) {
        return nullptr;
    }
    return &iter->second;
}

CellKind World::kind_at(const HexCoord coord) const noexcept {
    const Cell* const cell = find_cell(coord);
    if (cell == nullptr) {
        return CellKind::Empty;
    }
    return cell->kind;
}

float World::energy_at(const HexCoord coord) const noexcept {
    const Cell* const cell = find_cell(coord);
    if (cell == nullptr) {
        return 0.0f;
    }
    return cell->energy;
}

void World::set_cell(const HexCoord coord, const CellKind kind) {
    if (kind == CellKind::Empty) {
        cells_.erase(coord);
        return;
    }
    cells_[coord] = Cell{kind, 0U, 1.0f, HexDirection::East};
}

void World::set_cell_with_energy(const HexCoord coord, const CellKind kind, float energy) {
    if (kind == CellKind::Empty) {
        cells_.erase(coord);
        return;
    }
    cells_[coord] = Cell{kind, 0U, energy, HexDirection::East};
}

void World::set_cell_full(const HexCoord coord, const Cell& cell) {
    if (cell.kind == CellKind::Empty) {
        cells_.erase(coord);
        return;
    }
    cells_[coord] = cell;
}

void World::clear_cell(const HexCoord coord) {
    cells_.erase(coord);
}

void World::add_energy(const HexCoord coord, float amount) {
    Cell* cell = find_cell_mut(coord);
    if (cell != nullptr) {
        cell->energy += amount;
        if (cell->energy > 100.0f) cell->energy = 100.0f;
        if (cell->energy < 0.0f) cell->energy = 0.0f;
    }
}

void World::set_facing(const HexCoord coord, HexDirection direction) {
    Cell* cell = find_cell_mut(coord);
    if (cell != nullptr) {
        cell->facing = direction;
    }
}

std::vector<HexCoord> World::occupied_coords() const {
    std::vector<HexCoord> coords;
    coords.reserve(cells_.size());

    for (const auto& [coord, cell] : cells_) {
        (void)cell;
        coords.push_back(coord);
    }

    std::sort(coords.begin(), coords.end(), [](const HexCoord left, const HexCoord right) {
        if (left.q != right.q) {
            return left.q < right.q;
        }
        return left.r < right.r;
    });

    return coords;
}

WorldBounds compute_bounds(const World& world) {
    WorldBounds bounds;
    const std::vector<HexCoord> coords = world.occupied_coords();

    if (coords.empty()) {
        return bounds;
    }

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
