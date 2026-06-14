#include "hex_engine/world.h"

#include <algorithm>

namespace hex_engine {

void World::clear() {
    // Clearing the sparse map returns us to a true blank slate.
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

CellKind World::kind_at(const HexCoord coord) const noexcept {
    const Cell* const cell = find_cell(coord);
    if (cell == nullptr) {
        return CellKind::Empty;
    }

    return cell->kind;
}

void World::set_cell(const HexCoord coord, const CellKind kind) {
    if (kind == CellKind::Empty) {
        // Empty is treated as deletion so the storage stays sparse.
        cells_.erase(coord);
        return;
    }

    // A write always replaces the current cell with a fresh age counter.
    cells_[coord] = Cell{kind, 0U};
}

void World::clear_cell(const HexCoord coord) {
    cells_.erase(coord);
}

std::vector<HexCoord> World::occupied_coords() const {
    std::vector<HexCoord> coords;
    coords.reserve(cells_.size());

    for (const auto& [coord, cell] : cells_) {
        (void)cell; // The coordinate list only needs the location, not the payload.
        coords.push_back(coord);
    }

    // Stable sorted order keeps debug output and save serialization predictable.
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

