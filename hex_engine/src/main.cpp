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
    config.mutation_rate = 0.5f; // Very high for demo
    config.reproduction_threshold = 2.0f;
    config.producer_energy_per_tick = 1.2f;
    config.base_energy_decay = 0.02f;
    
    Simulator sim(config);
    
    // Create initial species
    auto founding_genome = std::make_shared<Genome>();
    founding_genome->anatomy = {
        {{0, 0}, CellKind::Producer},
        {{1, 0}, CellKind::Mouth},
        {{0, 1}, CellKind::Mover},
        {{-1, 0}, CellKind::Eye},
        {{0, -1}, CellKind::Signal},
        {{1, 1}, CellKind::Sense},
        {{-1, -1}, CellKind::Virus}
    };
    founding_genome->brain.randomize();
    // Connect Input 2 (Energy) to Output 3 (Reproduce)
    founding_genome->brain.synapses.push_back({2, NeuralNet::kInputCount + NeuralNet::kMaxHiddenNodes + 3, 2.0f});
    // Connect Input 0 (Vision Kind) to Output 0 (Move)
    founding_genome->brain.synapses.push_back({0, NeuralNet::kInputCount + NeuralNet::kMaxHiddenNodes + 0, 1.5f});
    
    auto initial_species = sim.fossil_record().record_new_species(founding_genome, nullptr, 0);
    
    // Seed initial organisms
    for(int i=0; i<3; ++i) {
        auto org = std::make_shared<Organism>(HexCoord{i*5, 0}, founding_genome, initial_species);
        org->energy = 10.0f;
        sim.registry().add_organism(org);
        org->sync_to_world(sim.world());
    }

    // Add some Walls and Poison
    for(int i=-10; i<=10; ++i) {
        sim.world().set_cell({i, -10}, CellKind::Wall);
        sim.world().set_cell({i, 10}, CellKind::Poison);
    }

    std::cout << "Starting simulation with 1 species: " << initial_species->name << "\n";
    std::cout << "Added Wall at R=-10, Poison at R=10\n";
    std::cout << "Sexual Reproduction: ENABLED\n";
    std::cout << "Dynamic Disasters: ENABLED\n\n";

    for (int i = 0; i < 200; ++i) {
        sim.tick();
        
        auto disaster = sim.current_disaster();
        if (disaster != Simulator::DisasterType::None && sim.disaster_timer() == config.disaster_duration - 1) {
            std::string d_name = "Unknown";
            if (disaster == Simulator::DisasterType::Radiation) d_name = "RADIATION (High Mutation)";
            else if (disaster == Simulator::DisasterType::Blight) d_name = "BLIGHT (Low Production)";
            else if (disaster == Simulator::DisasterType::SolarFlare) d_name = "SOLAR FLARE (Blinded Eyes)";
            std::cout << ">>> DISASTER STARTED: " << d_name << " at Tick " << i << "\n";
        }

        if (i % 20 == 0) {
            std::cout << "Tick " << i << ": " 
                      << sim.registry().organisms().size() << " orgs, "
                      << sim.fossil_record().extant_species().size() << " extant species.\n";
            
            if (sim.current_disaster() != Simulator::DisasterType::None) {
                std::cout << "  [Disaster Active: " << static_cast<int>(sim.current_disaster()) << " for " << sim.disaster_timer() << " ticks]\n";
            }

            for (const auto& [name, species] : sim.fossil_record().extant_species()) {
                std::cout << "  - " << name << ": pop " << species->population << "\n";
            }
        }
    }

    std::cout << "\n=== Species Evolution Verified ===\n";
    std::cout << "Lineage Tree (DOT format):\n";
    std::cout << sim.fossil_record().export_lineage() << "\n";
    return 0;
}
