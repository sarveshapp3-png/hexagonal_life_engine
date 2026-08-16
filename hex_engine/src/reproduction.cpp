#include "hex_engine/reproduction.h"

#include <algorithm>
#include <random>

namespace hex_engine {

std::uint32_t ReproductionSystem::process_reproduction(
    World& world,
    const OrganismRegistry& registry,
    const ReproductionConfig& config) {
    
    std::uint32_t total_offspring = 0;
    
    // Process each organism for reproduction.
    for (const auto& organism : registry.organisms()) {
        if (total_offspring >= config.max_offspring_per_tick) {
            break;
        }
        
        // Check if organism has enough energy to reproduce.
        if (organism.total_energy < config.reproduction_threshold) {
            continue;
        }
        
        // Try to spawn offspring.
        std::uint32_t offspring_count = 0;
        const std::uint32_t max_per_organism = 
            std::min(std::uint32_t(3), config.max_offspring_per_tick - total_offspring);
        
        for (std::uint32_t i = 0; i < max_per_organism; ++i) {
            if (try_spawn_offspring(world, organism, organism.cells[0] != HexCoord{} ? 
                                   world.kind_at(organism.cells[0]) : CellKind::Producer,
                                   config.offspring_energy, config.parent_energy_cost)) {
                ++offspring_count;
                ++total_offspring;
            } else {
                break;  // No more spawn sites available.
            }
        }
    }
    
    return total_offspring;
}

bool ReproductionSystem::try_spawn_offspring(World& world, const OrganismCluster& organism,
                                           CellKind offspring_kind, float offspring_energy,
                                           float parent_cost) {
    // Find available spawn sites.
    const auto spawn_sites = find_spawn_sites(world, organism);
    
    if (spawn_sites.empty()) {
        return false;
    }
    
    // Pick a random spawn site.
    static std::mt19937 rng(std::random_device{}());
    std::uniform_int_distribution<std::size_t> dist(0, spawn_sites.size() - 1);
    const HexCoord spawn_site = spawn_sites[dist(rng)];
    
    // Create the offspring cell.
    world.set_cell_with_energy(spawn_site, offspring_kind, offspring_energy);
    
    // Deduct energy cost from the parent organism.
    // Distribute the cost among the organism's cells.
    const float cost_per_cell = parent_cost / organism.cells.size();
    for (const HexCoord cell_coord : organism.cells) {
        world.add_energy(cell_coord, -cost_per_cell);
    }
    
    return true;
}

std::vector<HexCoord> ReproductionSystem::find_spawn_sites(
    const World& world,
    const OrganismCluster& organism) const {
    
    std::vector<HexCoord> sites;
    
    // Check all neighbors of all cells in the organism.
    for (const HexCoord cell_coord : organism.cells) {
        for (std::size_t dir_index = 0; dir_index < 6; ++dir_index) {
            const HexDirection dir = static_cast<HexDirection>(dir_index);
            const HexCoord neighbor = hex_neighbor(cell_coord, dir);
            
            // Spawn site must be empty.
            if (world.kind_at(neighbor) == CellKind::Empty) {
                // Avoid duplicates.
                if (std::find(sites.begin(), sites.end(), neighbor) == sites.end()) {
                    sites.push_back(neighbor);
                }
            }
        }
    }
    
    return sites;
}

} // namespace hex_engine
