#include "hex_engine/vision.h"

namespace hex_engine {

std::optional<VisionHit> VisionSystem::raycast(const World& world, HexCoord origin, 
                                              HexDirection direction, int max_range) const {
    HexCoord current = origin;
    for (int dist = 1; dist <= max_range; ++dist) {
        current = hex_neighbor(current, direction);
        CellKind kind = world.kind_at(current);
        if (kind != CellKind::Empty) {
            return VisionHit{current, kind, dist};
        }
    }
    return std::nullopt;
}

std::vector<VisionHit> VisionSystem::query_neighborhood(const World& world, HexCoord origin, 
                                                       int max_range) const {
    std::vector<VisionHit> hits;
    // This is a simple brute-force check for sparse worlds.
    // In a dense world, we'd iterate over the hex ring.
    for (const auto& coord : world.occupied_coords()) {
        int dist = hex_distance(origin, coord);
        if (dist > 0 && dist <= max_range) {
            hits.push_back({coord, world.kind_at(coord), dist});
        }
    }
    return hits;
}

} // namespace hex_engine
