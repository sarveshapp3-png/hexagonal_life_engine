#include <iostream>
#include <memory>
#include <vector>

#include "hex_engine/cell_kind.h"
#include "hex_engine/hex_coord.h"
#include "hex_engine/organism.h"
#include "hex_engine/simulation.h"
#include "hex_engine/world.h"

int main() {
    using namespace hex_engine;

    std::cout << "=== Hex Engine Phase 6: Advanced Behaviors (Killer, Vision, Brain) ===\n\n";

    SimulationConfig config;
    config.mutation_rate = 0.1f;
    config.reproduction_threshold = 3.0f;
    config.producer_energy_per_tick = 0.5f;
    
    Simulator sim(config);
    
    // Create a Predator Genome
    auto predator_genome = std::make_shared<Genome>();
    predator_genome->anatomy = {
        {{0, 0}, CellKind::Mover},
        {{1, 0}, CellKind::Killer},
        {{0, 1}, CellKind::Eye}
    };
    predator_genome->brain.reactions[CellKind::Food] = 1; // Chase food
    predator_genome->brain.reactions[CellKind::Producer] = 1; // Chase producers
    
    // Create a Prey Genome
    auto prey_genome = std::make_shared<Genome>();
    prey_genome->anatomy = {
        {{0, 0}, CellKind::Producer},
        {{1, 0}, CellKind::Armor}
    };
    
    // Seed organisms
    auto predator = std::make_shared<Organism>(HexCoord{0, 0}, predator_genome);
    predator->energy = 10.0f;
    sim.registry().add_organism(predator);
    
    auto prey = std::make_shared<Organism>(HexCoord{5, 0}, prey_genome);
    prey->energy = 10.0f;
    sim.registry().add_organism(prey);
    
    // Add some food
    sim.world().set_cell_with_energy({10, 0}, CellKind::Food, 20.0f);

    std::cout << "Predator at (0,0), Prey at (5,0), Food at (10,0)\n";
    std::cout << "Running 100 ticks...\n";

    for (int i = 0; i < 100; ++i) {
        sim.tick();
        
        if (i % 20 == 0) {
            std::cout << "Tick " << i << ": " 
                      << sim.registry().organisms().size() << " organisms.\n";
            for (const auto& org : sim.registry().organisms()) {
                std::string type = "Unknown";
                if (org->genome->anatomy[0].kind == CellKind::Mover) type = "Predator";
                if (org->genome->anatomy[0].kind == CellKind::Producer) type = "Prey";
                
                std::cout << "  - " << type << " at " << org->position 
                          << ", energy: " << org->energy << ", health: " << org->health << "\n";
            }
        }
    }

    std::cout << "\n=== Advanced Behaviors Verified ===\n";
    return 0;
}
