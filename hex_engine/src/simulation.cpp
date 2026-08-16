#include "hex_engine/simulation.h"

namespace hex_engine {

Simulator::Simulator(SimulationConfig config)
    : config_(config), tick_count_(0) {
}

void Simulator::tick() {
    // 1. Update all organisms
    organism_registry_.update_all(world_, config_);
    
    // 2. Process pheromones
    world_.diffuse_pheromones(0.1f); // 10% diffusion
    world_.decay_pheromones(0.95f);  // 5% decay
    
    ++tick_count_;
}

} // namespace hex_engine
