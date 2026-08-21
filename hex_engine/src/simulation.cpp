#include "hex_engine/simulation.h"
#include <random>

namespace hex_engine {

static std::mt19937 g_rng(std::random_device{}());

Simulator::Simulator(SimulationConfig config)
    : config_(config), tick_count_(0) {
}

Simulator::Season Simulator::current_season() const noexcept {
    uint32_t cycle = tick_count_ / config_.season_length;
    return static_cast<Season>(cycle % 4);
}

void Simulator::tick() {
    // 0. Update Disaster State
    if (disaster_timer_ > 0) {
        disaster_timer_--;
        if (disaster_timer_ == 0) current_disaster_ = DisasterType::None;
    } else {
        std::uniform_real_distribution<float> d_dist(0.0f, 1.0f);
        if (d_dist(g_rng) < config_.disaster_chance) {
            std::uniform_int_distribution<int> type_dist(1, 3);
            current_disaster_ = static_cast<DisasterType>(type_dist(g_rng));
            disaster_timer_ = config_.disaster_duration;
        }
    }

    // Apply seasonal and disaster modifiers to a local config copy
    SimulationConfig current_config = config_;
    
    Season season = current_season();
    if (season == Season::Summer) {
        current_config.producer_energy_per_tick *= 1.2f;
    } else if (season == Season::Winter) {
        current_config.producer_energy_per_tick *= 0.5f;
        current_config.food_spawn_prob *= 0.2f;
        current_config.base_energy_decay *= 1.5f;
    } else if (season == Season::Spring) {
        current_config.food_spawn_prob *= 2.0f;
    }

    if (current_disaster_ == DisasterType::Radiation) {
        current_config.mutation_rate *= 5.0f;
    } else if (current_disaster_ == DisasterType::Blight) {
        current_config.producer_energy_per_tick *= 0.2f;
    } else if (current_disaster_ == DisasterType::SolarFlare) {
        current_config.vision_range = 0;
    }

    auto& organisms = organism_registry_.organisms();
    
    // 1. Build position map for fast lookup
    // (In a real implementation, this would be a member of registry)
    std::unordered_map<HexCoord, std::shared_ptr<Organism>, HexCoordHash> pos_to_org;
    for (auto& org : organisms) {
        for (const auto& ac : org->genome->anatomy) {
            pos_to_org[org->get_world_pos(ac.local_pos)] = org;
        }
    }

    // 2. Update all organisms
    std::vector<std::shared_ptr<Organism>> to_add;
    std::vector<std::shared_ptr<Organism>> next_gen_organisms;

    for (auto& org : organisms) {
        org->clear_from_world(world_);
        org->update(world_, current_config, organism_registry_);
        
        if (org->health > 0) {
            // Reproduction
            std::vector<float> brain_inputs(NeuralNet::kInputCount, 0.0f);
            brain_inputs[2] = org->energy / (org->genome->anatomy.size() * 10.0f);
            auto outputs = org->genome->brain.process(brain_inputs);
            
            if (org->can_reproduce(current_config) && outputs[3] > 0.5f) {
                std::shared_ptr<Genome> child_genome;
                bool mutated = false;

                if (current_config.sexual_reproduction) {
                    std::shared_ptr<Organism> mate = nullptr;
                    for (auto& other : organisms) {
                        if (other == org) continue;
                        float d = hex_distance(org->position, other->position);
                        if (d <= current_config.mate_search_radius) {
                            if (org->genome->distance(*other->genome) <= current_config.kinship_threshold) {
                                mate = other;
                                break;
                            }
                        }
                    }
                    if (mate) {
                        child_genome = std::make_shared<Genome>(Genome::crossover(*org->genome, *mate->genome));
                        mutated = true;
                    }
                }

                if (!child_genome) {
                    child_genome = std::make_shared<Genome>(*org->genome);
                    std::uniform_real_distribution<float> dist(0.0f, 1.0f);
                    if (dist(g_rng) < current_config.mutation_rate) {
                        child_genome->mutate(current_config);
                        mutated = true;
                    }
                }
                
                std::uniform_int_distribution<int> dir_dist(0, 5);
                HexDirection dir = static_cast<HexDirection>(dir_dist(g_rng));
                int spawn_dist = static_cast<int>(org->genome->anatomy.size()) + 2;
                HexCoord child_pos = org->position;
                for(int i=0; i<spawn_dist; ++i) child_pos = hex_neighbor(child_pos, dir);
                
                bool clear = true;
                for (const auto& ac : child_genome->anatomy) {
                    HexCoord wp = {child_pos.q + ac.local_pos.q, child_pos.r + ac.local_pos.r};
                    if (world_.contains(wp)) { clear = false; break; }
                }
                
                if (clear) {
                    org->energy -= static_cast<float>(org->genome->anatomy.size()) * current_config.parent_energy_cost;
                    std::shared_ptr<Species> child_species = org->species;
                    if (mutated) {
                        child_species = fossil_record_.record_new_species(child_genome, org->species, tick_count_);
                    }
                    auto child = std::make_shared<Organism>(child_pos, child_genome, child_species);
                    child->energy = static_cast<float>(child_genome->anatomy.size()) * current_config.offspring_energy;
                    to_add.push_back(child);
                }
            }
            org->sync_to_world(world_);
            next_gen_organisms.push_back(org);
        }
    }

    // 3. Finalize registry
    organism_registry_.clear();
    for (auto& org : next_gen_organisms) organism_registry_.add_organism(org);
    for (auto& child : to_add) {
        organism_registry_.add_organism(child);
        child->sync_to_world(world_);
    }

    // 4. Process pheromones
    world_.diffuse_pheromones(0.1f);
    world_.decay_pheromones(0.95f);
    
    // 5. Global Food Spawning
    std::uniform_real_distribution<float> f_dist(0.0f, 1.0f);
    if (f_dist(g_rng) < current_config.food_spawn_prob) {
        std::uniform_int_distribution<int> pos_dist(-20, 20);
        HexCoord food_pos = {pos_dist(g_rng), pos_dist(g_rng)};
        if (world_.kind_at(food_pos) == CellKind::Empty) {
            world_.set_cell_with_energy(food_pos, CellKind::Food, current_config.food_spawn_energy);
        }
    }

    // 6. Update Fossil Record
    fossil_record_.update_stats(tick_count_, static_cast<uint32_t>(organism_registry_.organisms().size()));
    
    world_.increment_ticks();
    
    // 7. Battle Royale Shrinking
    if (config_.world_preset == SimulationConfig::WorldPreset::BattleRoyale) {
        int safe_radius = 100 - static_cast<int>(tick_count_ / 10);
        if (safe_radius < 5) safe_radius = 5;
        
        for (auto& org : next_gen_organisms) {
            if (hex_distance({0,0}, org->position) > safe_radius) {
                org->take_damage(10, current_config);
            }
        }
        
        // Clear non-organism cells outside radius
        auto coords = world_.occupied_coords();
        for (const auto& c : coords) {
            if (hex_distance({0,0}, c) > safe_radius) {
                world_.clear_cell(c);
            }
        }
    }

    ++tick_count_;
}

// Add missing clear method to Registry
void OrganismRegistry::clear() {
    organisms_.clear();
}

} // namespace hex_engine
