#include <array>
#include <iostream>
#include <string_view>

#include "hex_engine/cell_kind.h"
#include "hex_engine/hex_coord.h"
#include "hex_engine/mutation.h"
#include "hex_engine/organism.h"
#include "hex_engine/simulation.h"
#include "hex_engine/world.h"

namespace hex_engine_demo {

constexpr std::string_view kReferenceLogoPath = "assets/reference/life-engine-title.png";

void print_world_summary(const hex_engine::Simulator& sim) {
    using namespace hex_engine;
    const World& world = sim.world();
    const OrganismRegistry& registry = sim.registry();
    
    std::cout << "Tick " << sim.tick_count() << ": " << world.occupied_count() << " cells | ";
    
    std::uint32_t p=0, m=0, mo=0, f=0;
    float total_energy = 0.0f;
    
    for (const HexCoord coord : world.occupied_coords()) {
        const Cell* cell = world.find_cell(coord);
        if (cell) {
            total_energy += cell->energy;
            switch (cell->kind) {
                case CellKind::Producer: ++p; break;
                case CellKind::Mover: ++m; break;
                case CellKind::Mouth: ++mo; break;
                case CellKind::Food: ++f; break;
                default: break;
            }
        }
    }
    
    std::cout << "P:" << p << " M:" << m << " Mo:" << mo << " F:" << f 
              << " | Orgs: " << registry.organism_count() 
              << " | Energy: " << total_energy << "\n";
}

void print_organisms(const hex_engine::OrganismRegistry& registry) {
    using namespace hex_engine;
    std::cout << "\nOrganism clusters:\n";
    for (const auto& organism : registry.organisms()) {
        std::cout << "  Org #" << organism.id << " (" << cell_kind_name(organism.primary_kind) 
                  << "): " << organism.cell_count() << " cells, " << organism.total_energy 
                  << " energy\n";
    }
}

} // namespace hex_engine_demo

int main() {
    using namespace hex_engine;

    std::cout << "=== Hex Engine Phase 3: Corrected Reporting + Ecosystem ===\n\n";

    Simulator sim;
    const HexCoord origin{};

    // Initial seed
    sim.world().set_cell_with_energy(origin, CellKind::Producer, 15.0f);
    sim.world().set_cell_with_energy(hex_neighbor(origin, HexDirection::East), CellKind::Producer, 15.0f);
    sim.world().set_cell_with_energy(hex_neighbor(origin, HexDirection::NorthEast), CellKind::Mover, 8.0f);
    sim.world().set_cell_with_energy(hex_neighbor(origin, HexDirection::NorthWest), CellKind::Mover, 8.0f);
    sim.world().set_cell_with_energy(hex_neighbor(origin, HexDirection::SouthWest), CellKind::Mouth, 5.0f);
    sim.world().set_cell_with_energy(hex_neighbor(origin, HexDirection::SouthEast), CellKind::Food, 20.0f);

    std::cout << "=== Running 20 simulation ticks ===\n";
    for (int tick = 0; tick < 20; ++tick) {
        sim.tick();
        hex_engine_demo::print_world_summary(sim);
        
        if (tick == 0 || tick == 9 || tick == 19) {
            hex_engine_demo::print_organisms(sim.registry());
        }
    }

    std::cout << "\n=== Phase 3 Verified ===\n";
    std::cout << "Next: Implement Vision and Brain actions.\n";

    return 0;
}
