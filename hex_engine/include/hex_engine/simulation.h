#pragma once

#include <cstdint>
#include <vector>

#include "hex_engine/hex_coord.h"
#include "hex_engine/brain.h"
#include "hex_engine/mutation.h"
#include "hex_engine/organism.h"
#include "hex_engine/vision.h"
#include "hex_engine/world.h"

namespace hex_engine {

struct SimulationConfig {
    std::uint32_t target_fps = 60;
    std::uint32_t max_ticks_per_frame = 1;
    bool wrap_world = true;
    
    float producer_energy_per_tick = 0.5f;
    float mover_energy_cost_per_step = 0.3f;
    float mouth_harvest_efficiency = 0.8f;
    float base_energy_decay = 0.1f;
    
    float reproduction_threshold = 10.0f;
    float offspring_energy = 2.0f;
    float parent_energy_cost = 3.0f;
    
    float mutation_rate = 0.1f;
    float energy_variance = 0.2f;
};

class Simulator {
public:
    explicit Simulator(SimulationConfig config = {});
    
    [[nodiscard]] World& world() noexcept { return world_; }
    [[nodiscard]] const World& world() const noexcept { return world_; }
    
    [[nodiscard]] const OrganismRegistry& registry() const noexcept { return organism_registry_; }
    
    void tick();
    
    [[nodiscard]] std::uint32_t tick_count() const noexcept { return tick_count_; }
    [[nodiscard]] const SimulationConfig& config() const noexcept { return config_; }
    void set_config(const SimulationConfig& config) noexcept { config_ = config; }
    
private:
    World world_;
    SimulationConfig config_;
    std::uint32_t tick_count_ = 0;
    OrganismRegistry organism_registry_;
    MutationSystem mutation_system_;
    VisionSystem vision_system_;
    SimpleBrain simple_brain_;
    
    // Helper methods for tick processing.
    void process_producers();
    void process_mouths();
    void process_movers();
    void process_movement();
    void process_reproduction();
    void apply_energy_decay();
};

} // namespace hex_engine
