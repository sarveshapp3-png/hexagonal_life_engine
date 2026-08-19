#include "hex_engine/organism.h"
#include "hex_engine/world.h"
#include "hex_engine/simulation.h"
#include "hex_engine/species.h"
#include <algorithm>
#include <random>

namespace hex_engine {

static std::mt19937 g_rng(std::random_device{}());

// --- NeuralNet Implementation ---

void NeuralNet::randomize() {
    std::uniform_int_distribution<int> source_dist(0, kInputCount + kMaxHiddenNodes - 1);
    std::uniform_int_distribution<int> target_dist(kInputCount, kInputCount + kMaxHiddenNodes + kOutputCount - 1);
    std::uniform_real_distribution<float> weight_dist(-2.0f, 2.0f);

    synapses.clear();
    int synapse_count = 10 + (g_rng() % 10);
    for (int i = 0; i < synapse_count; ++i) {
        synapses.push_back({source_dist(g_rng), target_dist(g_rng), weight_dist(g_rng)});
    }
}

void NeuralNet::mutate(float rate) {
    std::uniform_real_distribution<float> dist(0.0f, 1.0f);
    std::uniform_real_distribution<float> weight_mut(-0.5f, 0.5f);
    
    for (auto& s : synapses) {
        if (dist(g_rng) < rate) s.weight += weight_mut(g_rng);
    }

    if (dist(g_rng) < rate) {
        // Add synapse
        std::uniform_int_distribution<int> source_dist(0, kInputCount + kMaxHiddenNodes - 1);
        std::uniform_int_distribution<int> target_dist(kInputCount, kInputCount + kMaxHiddenNodes + kOutputCount - 1);
        std::uniform_real_distribution<float> weight_dist(-2.0f, 2.0f);
        synapses.push_back({source_dist(g_rng), target_dist(g_rng), weight_dist(g_rng)});
    }

    if (dist(g_rng) < rate && !synapses.empty()) {
        // Remove synapse
        synapses.erase(synapses.begin() + (g_rng() % synapses.size()));
    }
}

std::vector<float> NeuralNet::process(const std::vector<float>& inputs) const {
    std::vector<float> nodes(kInputCount + kMaxHiddenNodes + kOutputCount, 0.0f);
    for (int i = 0; i < kInputCount; ++i) nodes[i] = inputs[i];

    // Simple one-pass feed-forward (assuming nodes are ordered roughly correctly)
    for (const auto& s : synapses) {
        nodes[s.target_node] += nodes[s.source_node] * s.weight;
    }

    // Activation
    std::vector<float> outputs(kOutputCount);
    for (int i = 0; i < kOutputCount; ++i) {
        outputs[i] = std::tanh(nodes[kInputCount + kMaxHiddenNodes + i]);
    }
    return outputs;
}

void Genome::mutate(float rate) {
    std::uniform_real_distribution<float> dist(0.0f, 1.0f);

    if (dist(g_rng) < rate && !anatomy.empty()) {
        std::uniform_int_distribution<size_t> idx_dist(0, anatomy.size() - 1);
        size_t idx = idx_dist(g_rng);
        std::uniform_int_distribution<int> kind_dist(1, 8);
        anatomy[idx].kind = static_cast<CellKind>(kind_dist(g_rng));
    }

    if (dist(g_rng) < rate && anatomy.size() > 1) {
        std::uniform_int_distribution<size_t> idx_dist(0, anatomy.size() - 1);
        size_t idx = idx_dist(g_rng);
        if (!(anatomy[idx].local_pos.q == 0 && anatomy[idx].local_pos.r == 0)) {
            anatomy.erase(anatomy.begin() + idx);
        }
    }

    if (dist(g_rng) < rate) {
        std::uniform_int_distribution<size_t> idx_dist(0, anatomy.size() - 1);
        size_t idx = idx_dist(g_rng);
        std::uniform_int_distribution<int> dir_dist(0, 5);
        HexDirection dir = static_cast<HexDirection>(dir_dist(g_rng));
        HexCoord new_pos = hex_neighbor(anatomy[idx].local_pos, dir);
        
        bool occupied = false;
        for (const auto& c : anatomy) {
            if (c.local_pos == new_pos) { occupied = true; break; }
        }
        if (!occupied) {
            std::uniform_int_distribution<int> kind_dist(1, 8);
            anatomy.push_back({new_pos, static_cast<CellKind>(kind_dist(g_rng))});
        }
    }
    brain.mutate(rate);
}

float Genome::distance(const Genome& other) const {
    float dist = 0.0f;
    dist += std::abs(static_cast<float>(anatomy.size()) - static_cast<float>(other.anatomy.size()));
    
    // Neural distance: simple synapse count difference + weight difference
    dist += std::abs(static_cast<float>(brain.synapses.size()) - static_cast<float>(other.brain.synapses.size()));
    
    size_t min_synapses = std::min(brain.synapses.size(), other.brain.synapses.size());
    for (size_t i = 0; i < min_synapses; ++i) {
        dist += std::abs(brain.synapses[i].weight - other.brain.synapses[i].weight);
    }
    
    return dist;
}

// --- Organism Implementation ---

Organism::Organism(HexCoord pos, std::shared_ptr<Genome> gen, std::shared_ptr<Species> spec)
    : position(pos), genome(gen), species(spec) {
    health = static_cast<int>(genome->anatomy.size());
    if (species) species->add_instance();
}

HexCoord rotate_hex(HexCoord coord, int steps) {
    steps = (steps % 6 + 6) % 6;
    HexCoord current = coord;
    for (int i = 0; i < steps; ++i) {
        int q = current.q;
        int r = current.r;
        current.q = -r;
        current.r = q + r;
    }
    return current;
}

HexCoord Organism::get_world_pos(HexCoord local_pos) const {
    HexCoord rotated = rotate_hex(local_pos, static_cast<int>(rotation));
    return {position.q + rotated.q, position.r + rotated.r};
}

void Organism::sync_to_world(World& world) const {
    for (const auto& ac : genome->anatomy) {
        world.set_cell_full(get_world_pos(ac.local_pos), {ac.kind, age, energy / genome->anatomy.size(), rotation});
    }
}

void Organism::clear_from_world(World& world) const {
    for (const auto& ac : genome->anatomy) {
        world.clear_cell(get_world_pos(ac.local_pos));
    }
}

void Organism::take_damage(int amount, const SimulationConfig& config) {
    if (config.insta_kill) { health = 0; return; }
    int armor = 0;
    for (const auto& ac : genome->anatomy) {
        if (ac.kind == CellKind::Armor) armor++;
    }
    health -= std::max(0, amount - armor);
}

void Organism::update(World& world, const SimulationConfig& config, OrganismRegistry& registry) {
    age++;
    float gained_energy = 0.0f;
    
    // Brain Inputs
    std::vector<float> brain_inputs(NeuralNet::kInputCount, 0.0f);
    
    bool has_mover = false;
    bool has_eye = false;
    bool has_sense = false;
    bool has_signal = false;

    for (const auto& ac : genome->anatomy) {
        HexCoord wp = get_world_pos(ac.local_pos);
        if (ac.kind == CellKind::Producer) {
            gained_energy += config.producer_energy_per_tick * world.light_at(wp, 0.05f);
        } else if (ac.kind == CellKind::Mouth) {
            for (int i = 0; i < 6; ++i) {
                HexCoord n = hex_neighbor(wp, static_cast<HexDirection>(i));
                if (world.kind_at(n) == CellKind::Food) {
                    float harvested = world.energy_at(n) * config.mouth_harvest_efficiency;
                    gained_energy += harvested;
                    world.add_energy(n, -harvested);
                    if (world.energy_at(n) < 0.1f) world.clear_cell(n);
                }
            }
        } else if (ac.kind == CellKind::Killer) {
            for (int i = 0; i < 6; ++i) {
                HexCoord n = hex_neighbor(wp, static_cast<HexDirection>(i));
                auto target = registry.get_organism_at(n);
                if (target && target.get() != this) target->take_damage(1, config);
            }
        } else if (ac.kind == CellKind::Mover) {
            has_mover = true;
        } else if (ac.kind == CellKind::Sense) {
            has_sense = true;
        } else if (ac.kind == CellKind::Eye && !has_eye) {
            has_eye = true;
            for (int d = 1; d <= config.vision_range; ++d) {
                HexCoord look_pos = wp;
                for(int k=0; k<d; ++k) look_pos = hex_neighbor(look_pos, rotation);
                CellKind seen = world.kind_at(look_pos);
                if (seen != CellKind::Empty) {
                    brain_inputs[0] = static_cast<float>(seen) / 10.0f; // Normalized kind
                    brain_inputs[1] = 1.0f - (static_cast<float>(d) / config.vision_range); // Normalized distance
                    break;
                }
            }
        } else if (ac.kind == CellKind::Signal) {
            has_signal = true;
            brain_inputs[5] += world.pheromone_at(wp, 0);
            brain_inputs[6] += world.pheromone_at(wp, 1);
            brain_inputs[7] += world.pheromone_at(wp, 2);
        } else if (ac.kind == CellKind::Virus) {
            for (int i = 0; i < 6; ++i) {
                HexCoord n = hex_neighbor(wp, static_cast<HexDirection>(i));
                auto target = registry.get_organism_at(n);
                if (target && target.get() != this) {
                    std::uniform_real_distribution<float> v_dist(0.0f, 1.0f);
                    if (v_dist(g_rng) < config.virus_infection_chance) {
                        target->genome = genome;
                        target->species = species;
                    }
                }
            }
        } else if (ac.kind == CellKind::Scavenger) {
            for (int i = 0; i < 6; ++i) {
                HexCoord n = hex_neighbor(wp, static_cast<HexDirection>(i));
                if (world.kind_at(n) == CellKind::Food) {
                    float harvested = world.energy_at(n) * 1.5f; // Scavengers are more efficient
                    gained_energy += harvested;
                    world.clear_cell(n);
                }
            }
        } else if (ac.kind == CellKind::Explosive) {
            if (health < static_cast<int>(genome->anatomy.size()) / 2) {
                // Explode!
                for (int q = -config.explosive_radius; q <= config.explosive_radius; ++q) {
                    for (int r = -config.explosive_radius; r <= config.explosive_radius; ++r) {
                        HexCoord target_pos = {wp.q + q, wp.r + r};
                        if (hex_distance(wp, target_pos) <= config.explosive_radius) {
                            auto target = registry.get_organism_at(target_pos);
                            if (target && target.get() != this) {
                                target->take_damage(static_cast<int>(config.explosive_damage), config);
                            }
                            if (world.kind_at(target_pos) == CellKind::Food) world.clear_cell(target_pos);
                        }
                    }
                }
                health = 0; // Kills self
            }
        }
    }

    if (has_sense) {
        brain_inputs[2] = energy / (genome->anatomy.size() * 10.0f); // Normalized energy
        brain_inputs[3] = static_cast<float>(age) / (genome->anatomy.size() * config.lifespan_multiplier); // Normalized age
        brain_inputs[4] = world.light_at(position, 0.05f); // Light at center
    }

    if (!has_eye) { brain_inputs[0] = 0.0f; brain_inputs[1] = 0.0f; }
    if (!has_signal) { brain_inputs[5] = 0.0f; brain_inputs[6] = 0.0f; brain_inputs[7] = 0.0f; }

    // Process Brain
    std::vector<float> brain_outputs = genome->brain.process(brain_inputs);
    
    // Handle Outputs
    if (has_mover) {
        energy -= config.mover_energy_cost_per_step;
        
        HexCoord move_offset = {0, 0};
        if (brain_outputs[0] > 0.5f) { // Move Forward
            move_offset = kHexDirectionOffsets[static_cast<int>(rotation)];
        }
        
        if (brain_outputs[1] > 0.5f) { // Rotate Left
            rotation = static_cast<HexDirection>((static_cast<int>(rotation) + 5) % 6);
        } else if (brain_outputs[2] > 0.5f) { // Rotate Right
            rotation = static_cast<HexDirection>((static_cast<int>(rotation) + 1) % 6);
        }

        if (move_offset.q != 0 || move_offset.r != 0) {
            HexCoord new_pos = {position.q + move_offset.q, position.r + move_offset.r};
            bool collision = false;
            for (const auto& ac : genome->anatomy) {
                HexCoord wp = {new_pos.q + ac.local_pos.q, new_pos.r + ac.local_pos.r};
                if (world.contains(wp) && world.kind_at(wp) != CellKind::Food) { collision = true; break; }
            }
            if (!collision) position = new_pos;
        }
    }

    if (brain_outputs[3] > 0.5f) { // Reproduce
        // This is handled by the Simulator, but we could set a flag here
    }

    if (brain_outputs[4] > 0.5f) { // Emit Signal
        for (const auto& ac : genome->anatomy) {
            if (ac.kind == CellKind::Signal) {
                world.add_pheromone(get_world_pos(ac.local_pos), 0, 1.0f);
            }
        }
    }

    energy += gained_energy;
    
    // Temperature-based decay
    float dist_from_center = std::sqrt(static_cast<float>(position.q * position.q + position.r * position.r));
    float temp = 1.0f - (dist_from_center * config.temperature_gradient);
    float temp_stress = std::abs(temp - config.ideal_temperature);
    
    energy -= genome->anatomy.size() * (config.base_energy_decay + temp_stress * 0.1f);

    if (energy <= 0.0f || age > genome->anatomy.size() * config.lifespan_multiplier || health <= 0) {
        health = 0;
        if (species) species->remove_instance();
        for (const auto& ac : genome->anatomy) {
            world.set_cell_with_energy(get_world_pos(ac.local_pos), CellKind::Food, 2.0f);
        }
    }
}

bool Organism::can_reproduce(const SimulationConfig& config) const {
    return energy >= static_cast<float>(genome->anatomy.size()) * config.reproduction_threshold;
}

std::shared_ptr<Organism> Organism::reproduce(World& world, const SimulationConfig& config) {
    (void)world; // Check logic moved to update_all for better registry sync
    return nullptr; 
}

// --- Registry Implementation ---

void OrganismRegistry::add_organism(std::shared_ptr<Organism> org) {
    organisms_.push_back(org);
}

std::shared_ptr<Organism> OrganismRegistry::get_organism_at(HexCoord wp) const {
    auto it = pos_to_org_.find(wp);
    if (it != pos_to_org_.end()) return it->second;
    return nullptr;
}

void OrganismRegistry::update_all(World& world, const SimulationConfig& config) {
    // Registry update logic handled in Simulator::tick for better FossilRecord access
}

} // namespace hex_engine
