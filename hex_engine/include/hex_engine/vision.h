#pragma once

#include <vector>
#include <optional>

#include "hex_engine/hex_coord.h"
#include "hex_engine/world.h"

namespace hex_engine {

// A vision hit records what was seen at a specific coordinate.
struct VisionHit {
    HexCoord coord;
    CellKind kind;
    int distance;
};

class VisionSystem {
public:
    VisionSystem() = default;
    
    // Look in a specific direction from a coordinate.
    // Returns the first non-empty cell hit within max_range.
    [[nodiscard]] std::optional<VisionHit> raycast(const World& world, HexCoord origin, 
                                                  HexDirection direction, int max_range) const;
    
    // Get all occupied cells within a certain distance.
    [[nodiscard]] std::vector<VisionHit> query_neighborhood(const World& world, HexCoord origin, 
                                                           int max_range) const;
};

} // namespace hex_engine
