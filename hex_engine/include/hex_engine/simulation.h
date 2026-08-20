#pragma once

#include <cstdint>
#include <vector>
#include <memory>

#include "hex_engine/hex_coord.h"
#include "hex_engine/organism.h"
#include "hex_engine/world.h"
#include "hex_engine/species.h"

namespace hex_engine {

struct SimulationConfig {
    std::uint32_t target_fps = 60;
    std::uint32_t max_ticks_per_frame = 1;
    bool wrap_world = true;
    
    float producer_energy_per_tick = 0.5f;
    float mover_energy_cost_per_step = 0.3f;
    float mouth_harvest_efficiency = 0.8f;
    float base_energy_decay = 0.05f;
    
    float reproduction_threshold = 2.0f; // per cell
    float offspring_energy = 1.0f;       // per cell
    float parent_energy_cost = 1.0f;     // per cell
    
    float mutation_rate = 0.1f;
    bool insta_kill = false;
    int lifespan_multiplier = 100;
    int vision_range = 5;
    
    float temperature_gradient = 0.02f;
    float ideal_temperature = 0.5f;
    
    float virus_infection_chance = 0.05f;
    int explosive_radius = 2;
    float explosive_damage = 50.0f;
    
    float food_spawn_prob = 0.05f;
    float food_spawn_energy = 10.0f;
    float poison_damage = 10.0f;
};

class Simulator {
public:
    explicit Simulator(SimulationConfig config = {});
    
    [[nodiscard]] World& world() noexcept { return world_; }
    [[nodiscard]] const World& world() const noexcept { return world_; }
    
    [[nodiscard]] OrganismRegistry& registry() noexcept { return organism_registry_; }
    [[nodiscard]] const OrganismRegistry& registry() const noexcept { return organism_registry_; }
    
    [[nodiscard]] FossilRecord& fossil_record() noexcept { return fossil_record_; }
    [[nodiscard]] const FossilRecord& fossil_record() const noexcept { return fossil_record_; }

    void tick();
    
    [[nodiscard]] std::uint32_t tick_count() const noexcept { return tick_count_; }
    [[nodiscard]] const SimulationConfig& config() const noexcept { return config_; }
    void set_config(const SimulationConfig& config) noexcept { config_ = config; }
    
private:
    World world_;
    SimulationConfig config_;
    std::uint32_t tick_count_ = 0;
    OrganismRegistry organism_registry_;
    FossilRecord fossil_record_;
};

} // namespace hex_engine
