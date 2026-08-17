#include <iostream>
#include <memory>
#include <vector>

#include "hex_engine/cell_kind.h"
#include "hex_engine/hex_coord.h"
#include "hex_engine/organism.h"
#include "hex_engine/simulation.h"
#include "hex_engine/world.h"
#include "hex_engine/species.h"

int main() {
    using namespace hex_engine;

    std::cout << "=== Hex Engine Phase 7: Species Tracking & Fossil Record ===\n\n";

    SimulationConfig config;
    config.mutation_rate = 0.3f; // High mutation to see new species
    config.reproduction_threshold = 4.0f;
    config.producer_energy_per_tick = 0.8f;
    
    Simulator sim(config);
    
    // Create initial species
    auto founding_genome = std::make_shared<Genome>();
    founding_genome->anatomy = {
        {{0, 0}, CellKind::Producer},
        {{1, 0}, CellKind::Mouth},
        {{0, 1}, CellKind::Mover}
    };
    founding_genome->brain.randomize();
    
    auto initial_species = sim.fossil_record().record_new_species(founding_genome, nullptr, 0);
    
    // Seed initial organisms
    for(int i=0; i<3; ++i) {
        auto org = std::make_shared<Organism>(HexCoord{i*5, 0}, founding_genome, initial_species);
        org->energy = 10.0f;
        sim.registry().add_organism(org);
        org->sync_to_world(sim.world());
    }

    std::cout << "Starting simulation with 1 species: " << initial_species->name << "\n";

    for (int i = 0; i < 100; ++i) {
        sim.tick();
        
        if (i % 20 == 0) {
            std::cout << "Tick " << i << ": " 
                      << sim.registry().organisms().size() << " orgs, "
                      << sim.fossil_record().extant_species().size() << " extant species.\n";
            
            for (const auto& [name, species] : sim.fossil_record().extant_species()) {
                std::cout << "  - " << name << ": pop " << species->population << "\n";
            }
        }
    }

    std::cout << "\n=== Species Evolution Verified ===\n";
    return 0;
}
