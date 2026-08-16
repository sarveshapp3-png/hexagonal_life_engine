#pragma once

#include <cstdint>
#include <vector>

#include "hex_engine/hex_coord.h"
#include "hex_engine/organism.h"
#include "hex_engine/world.h"

namespace hex_engine {

// Reproduction configuration.
struct ReproductionConfig {
    float reproduction_threshold = 10.0f;  // Energy required to spawn a new cell
    float offspring_energy = 2.0f;         // Energy given to new cell
    float parent_energy_cost = 3.0f;       // Energy cost to parent for reproduction
    std::uint32_t max_offspring_per_tick = 3;  // Limit reproduction rate
};

// Reproduction system handles organism growth and cell spawning.
class ReproductionSystem {
public:
    ReproductionSystem() = default;
    
    // Process reproduction for all organisms in the world.
    // Returns the number of new cells created.
    [[nodiscard]] std::uint32_t process_reproduction(
        World& world,
        const OrganismRegistry& registry,
        const ReproductionConfig& config);
    
private:
    // Try to spawn a new cell adjacent to an organism.
    // Returns true if successful.
    bool try_spawn_offspring(World& world, const OrganismCluster& organism,
                            CellKind offspring_kind, float offspring_energy,
                            float parent_cost);
    
    // Find an empty neighbor adjacent to the organism.
    [[nodiscard]] std::vector<HexCoord> find_spawn_sites(
        const World& world,
        const OrganismCluster& organism) const;
};

} // namespace hex_engine
