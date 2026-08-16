#include "hex_engine/organism.h"

#include <queue>

namespace hex_engine {

void OrganismRegistry::rebuild_from_world(const World& world) {
    organisms_.clear();
    cell_to_organism_id_.clear();
    next_organism_id_ = 1;
    
    std::unordered_map<HexCoord, bool, HexCoordHash> visited;
    
    for (const HexCoord coord : world.occupied_coords()) {
        if (visited[coord]) continue;
        cluster_from_seed(coord, world, visited);
    }
}

void OrganismRegistry::cluster_from_seed(const HexCoord seed, const World& world,
                                        std::unordered_map<HexCoord, bool, HexCoordHash>& visited) {
    OrganismCluster organism;
    organism.id = next_organism_id_++;
    
    std::queue<HexCoord> queue;
    queue.push(seed);
    visited[seed] = true;
    
    CellKind seed_kind = world.kind_at(seed);
    
    while (!queue.empty()) {
        const HexCoord current = queue.front();
        queue.pop();
        
        organism.cells.push_back(current);
        
        for (std::size_t dir_index = 0; dir_index < 6; ++dir_index) {
            const HexDirection dir = static_cast<HexDirection>(dir_index);
            const HexCoord neighbor = hex_neighbor(current, dir);
            
            if (!visited[neighbor] && 
                world.kind_at(neighbor) == seed_kind &&
                world.kind_at(neighbor) != CellKind::Empty) {
                
                visited[neighbor] = true;
                queue.push(neighbor);
            }
        }
    }
    
    organism.primary_kind = seed_kind;
    
    for (const HexCoord coord : organism.cells) {
        const Cell* cell = world.find_cell(coord);
        if (cell) {
            organism.total_energy += cell->energy;
            organism.age = std::max(organism.age, cell->age);
            cell_to_organism_id_[coord] = organism.id;
        }
    }
    
    organisms_.push_back(organism);
}

const OrganismCluster* OrganismRegistry::find_organism_at(const HexCoord coord) const noexcept {
    const auto iter = cell_to_organism_id_.find(coord);
    if (iter == cell_to_organism_id_.end()) return nullptr;
    return find_organism_by_id(iter->second);
}

const OrganismCluster* OrganismRegistry::find_organism_by_id(std::uint32_t id) const noexcept {
    for (const auto& organism : organisms_) {
        if (organism.id == id) return &organism;
    }
    return nullptr;
}

} // namespace hex_engine
