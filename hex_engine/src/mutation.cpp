#include "hex_engine/mutation.h"

#include <random>

namespace hex_engine {

MutationSystem::MutationSystem() : rng_(std::random_device{}()) {
}

CellKind MutationSystem::mutate_cell_kind(CellKind parent_kind, const MutationConfig& config) {
    std::uniform_real_distribution<float> chance(0.0f, 1.0f);
    if (chance(rng_) > config.mutation_rate) return parent_kind;
    
    std::uniform_int_distribution<int> cell_dist(0, 8);
    return static_cast<CellKind>(cell_dist(rng_));
}

float MutationSystem::mutate_energy(float parent_energy, const MutationConfig& config) {
    std::uniform_real_distribution<float> variance(-config.energy_variance, config.energy_variance);
    float factor = 1.0f + variance(rng_);
    float mutated = parent_energy * factor;
    
    if (mutated < 0.5f) mutated = 0.5f;
    if (mutated > 10.0f) mutated = 10.0f;
    
    return mutated;
}

} // namespace hex_engine
