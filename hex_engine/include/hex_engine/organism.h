#pragma once

#include <cstdint>
#include <unordered_map>
#include <vector>

#include "hex_engine/hex_coord.h"
#include "hex_engine/world.h"

namespace hex_engine {

// An organism is a contiguous cluster of cells connected via hex neighbors.
struct OrganismCluster {
    std::uint32_t id = 0;
    std::vector<HexCoord> cells;
    float total_energy = 0.0f;
    std::uint32_t age = 0;
    CellKind primary_kind = CellKind::Empty;
    
    [[nodiscard]] std::size_t cell_count() const noexcept { return cells.size(); }
};

// OrganismRegistry tracks all organisms in the world.
class OrganismRegistry {
public:
    OrganismRegistry() = default;
    
    // Rebuild the organism list from the current world state.
    void rebuild_from_world(const World& world);
    
    // Get all organisms.
    [[nodiscard]] const std::vector<OrganismCluster>& organisms() const noexcept {
        return organisms_;
    }
    
    // Find the organism containing a given cell.
    [[nodiscard]] const OrganismCluster* find_organism_at(const HexCoord coord) const noexcept;
    
    // Get organism by ID.
    [[nodiscard]] const OrganismCluster* find_organism_by_id(std::uint32_t id) const noexcept;
    
    // Get the count of organisms.
    [[nodiscard]] std::size_t organism_count() const noexcept { return organisms_.size(); }
    
private:
    std::vector<OrganismCluster> organisms_;
    std::unordered_map<HexCoord, std::uint32_t, HexCoordHash> cell_to_organism_id_;
    std::uint32_t next_organism_id_ = 1;
    
    void cluster_from_seed(const HexCoord seed, const World& world,
                          std::unordered_map<HexCoord, bool, HexCoordHash>& visited);
};

} // namespace hex_engine
