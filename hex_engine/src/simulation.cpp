#include "hex_engine/simulation.h"

#include <algorithm>
#include <cmath>

namespace hex_engine {

Simulator::Simulator(SimulationConfig config)
    : config_(config), tick_count_(0) {
}

void Simulator::tick() {
    process_producers();
    process_mouths();
    process_movers();
    
    // Brains decide actions based on vision.
    const auto coords = world_.occupied_coords();
    for (const HexCoord coord : coords) {
        Cell* cell = world_.find_cell_mut(coord);
        if (cell && (cell->kind == CellKind::Mover || cell->kind == CellKind::Eye)) {
            auto vision = vision_system_.query_neighborhood(world_, coord, 3);
            Action action = simple_brain_.decide(*cell, vision);
            
            if (action.kind == ActionKind::RotateClockwise) {
                cell->facing = static_cast<HexDirection>((static_cast<int>(cell->facing) + 1) % 6);
            } else if (action.kind == ActionKind::RotateCounterClockwise) {
                cell->facing = static_cast<HexDirection>((static_cast<int>(cell->facing) + 5) % 6);
            }
            // Other actions handled in specific process steps.
        }
    }
    
    process_movement();
    process_reproduction();
    apply_energy_decay();
    
    organism_registry_.rebuild_from_world(world_);
    ++tick_count_;
}

void Simulator::process_producers() {
    const auto coords = world_.occupied_coords();
    for (const HexCoord coord : coords) {
        if (world_.kind_at(coord) == CellKind::Producer) {
            world_.add_energy(coord, config_.producer_energy_per_tick);
        }
    }
}

void Simulator::process_mouths() {
    const auto coords = world_.occupied_coords();
    for (const HexCoord coord : coords) {
        if (world_.kind_at(coord) == CellKind::Mouth) {
            for (std::size_t dir_index = 0; dir_index < 6; ++dir_index) {
                const HexDirection dir = static_cast<HexDirection>(dir_index);
                const HexCoord neighbor = hex_neighbor(coord, dir);
                
                if (world_.kind_at(neighbor) == CellKind::Food) {
                    float food_energy = world_.energy_at(neighbor);
                    float harvested = food_energy * config_.mouth_harvest_efficiency;
                    
                    world_.add_energy(coord, harvested);
                    world_.add_energy(neighbor, -harvested);
                    
                    if (world_.energy_at(neighbor) <= 0.01f) {
                        world_.clear_cell(neighbor);
                    }
                }
            }
        }
    }
}

void Simulator::process_movers() {
    const auto coords = world_.occupied_coords();
    for (const HexCoord coord : coords) {
        if (world_.kind_at(coord) == CellKind::Mover) {
            world_.add_energy(coord, -config_.mover_energy_cost_per_step);
        }
    }
}

void Simulator::process_movement() {
    const auto coords = world_.occupied_coords();
    for (const HexCoord coord : coords) {
        if (world_.kind_at(coord) == CellKind::Mover) {
            const Cell* cell = world_.find_cell(coord);
            if (cell != nullptr && cell->energy >= config_.mover_energy_cost_per_step) {
                const HexCoord target = hex_neighbor(coord, cell->facing);
                const CellKind target_kind = world_.kind_at(target);
                
                if (target_kind == CellKind::Empty || target_kind == CellKind::Food) {
                    Cell moving_cell = *cell;
                    moving_cell.energy -= config_.mover_energy_cost_per_step;
                    moving_cell.age += 1;
                    
                    world_.set_cell_full(target, moving_cell);
                    world_.clear_cell(coord);
                }
            }
        }
    }
}

void Simulator::process_reproduction() {
    const auto coords = world_.occupied_coords();
    std::vector<HexCoord> coords_copy = coords;
    
    MutationConfig mutation_config;
    mutation_config.mutation_rate = config_.mutation_rate;
    mutation_config.energy_variance = config_.energy_variance;
    
    for (const HexCoord coord : coords_copy) {
        if (world_.kind_at(coord) == CellKind::Empty) continue;
        
        float energy = world_.energy_at(coord);
        if (energy >= config_.reproduction_threshold) {
            for (std::size_t dir_index = 0; dir_index < 6; ++dir_index) {
                const HexDirection dir = static_cast<HexDirection>(dir_index);
                const HexCoord spawn_site = hex_neighbor(coord, dir);
                
                if (world_.kind_at(spawn_site) == CellKind::Empty) {
                    CellKind parent_kind = world_.kind_at(coord);
                    CellKind offspring_kind = mutation_system_.mutate_cell_kind(parent_kind, mutation_config);
                    float offspring_energy = mutation_system_.mutate_energy(config_.offspring_energy, mutation_config);
                    
                    world_.set_cell_with_energy(spawn_site, offspring_kind, offspring_energy);
                    world_.add_energy(coord, -config_.parent_energy_cost);
                    break;
                }
            }
        }
    }
}

void Simulator::apply_energy_decay() {
    const auto coords = world_.occupied_coords();
    for (const HexCoord coord : coords) {
        if (world_.kind_at(coord) != CellKind::Empty) {
            float current_energy = world_.energy_at(coord);
            float decay = current_energy * config_.base_energy_decay;
            world_.add_energy(coord, -decay);
            
            if (world_.energy_at(coord) <= 0.01f) {
                world_.clear_cell(coord);
            }
        }
    }
}

} // namespace hex_engine
