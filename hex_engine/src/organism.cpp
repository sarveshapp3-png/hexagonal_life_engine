#include "hex_engine/organism.h"
#include "hex_engine/world.h"
#include "hex_engine/simulation.h"
#include "hex_engine/species.h"
#include <algorithm>
#include <random>

namespace hex_engine {

static std::mt19937 g_rng(std::random_device{}());

// --- Genome & Brain Implementation ---

void BrainParams::randomize() {
    std::uniform_int_distribution<int> react_dist(0, 2);
    for (int i = 0; i <= 8; ++i) {
        reactions[static_cast<CellKind>(i)] = react_dist(g_rng);
    }
}

void BrainParams::mutate(float rate) {
    std::uniform_real_distribution<float> dist(0.0f, 1.0f);
    std::uniform_int_distribution<int> react_dist(0, 2);
    for (auto& [kind, react] : reactions) {
        if (dist(g_rng) < rate) {
            react = react_dist(g_rng);
        }
    }
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
    // Anatomy distance: simple cell count difference + mismatched cells
    dist += std::abs(static_cast<float>(anatomy.size()) - static_cast<float>(other.anatomy.size()));
    
    // Brain distance: number of different reactions
    for (auto const& [kind, react] : brain.reactions) {
        auto it = other.brain.reactions.find(kind);
        if (it == other.brain.reactions.end() || it->second != react) {
            dist += 1.0f;
        }
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
    bool has_mover = false;
    HexCoord move_dir = {0, 0};
    bool decided_move = false;

    for (const auto& ac : genome->anatomy) {
        HexCoord wp = get_world_pos(ac.local_pos);
        if (ac.kind == CellKind::Producer) {
            gained_energy += config.producer_energy_per_tick;
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
        } else if (ac.kind == CellKind::Eye && !decided_move) {
            for (int d = 1; d <= config.vision_range; ++d) {
                HexCoord look_pos = wp;
                for(int k=0; k<d; ++k) look_pos = hex_neighbor(look_pos, rotation);
                CellKind seen = world.kind_at(look_pos);
                if (seen != CellKind::Empty) {
                    int reaction = genome->brain.reactions[seen];
                    if (reaction == 1) { // Chase
                        move_dir = kHexDirectionOffsets[static_cast<int>(rotation)];
                        decided_move = true;
                    } else if (reaction == 2) { // Retreat
                        HexDirection opp = static_cast<HexDirection>((static_cast<int>(rotation) + 3) % 6);
                        move_dir = kHexDirectionOffsets[static_cast<int>(opp)];
                        decided_move = true;
                    }
                    break;
                }
            }
        }
    }

    if (has_mover) {
        energy -= config.mover_energy_cost_per_step;
        if (decided_move) {
            HexCoord new_pos = {position.q + move_dir.q, position.r + move_dir.r};
            bool collision = false;
            for (const auto& ac : genome->anatomy) {
                HexCoord wp = {new_pos.q + ac.local_pos.q, new_pos.r + ac.local_pos.r};
                if (world.contains(wp) && world.kind_at(wp) != CellKind::Food) { collision = true; break; }
            }
            if (!collision) position = new_pos;
        } else {
            std::uniform_int_distribution<int> dir_dist(0, 5);
            HexCoord rm = kHexDirectionOffsets[dir_dist(g_rng)];
            position = {position.q + rm.q, position.r + rm.r};
        }
    }

    energy += gained_energy;
    energy -= genome->anatomy.size() * config.base_energy_decay;

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
