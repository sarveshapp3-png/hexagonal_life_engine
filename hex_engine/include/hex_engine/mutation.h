#pragma once

#include <cstdint>
#include <random>

#include "hex_engine/cell_kind.h"
#include "hex_engine/hex_coord.h"

namespace hex_engine {

struct MutationConfig {
    float mutation_rate = 0.1f;
    float energy_variance = 0.2f;
};

class MutationSystem {
public:
    MutationSystem();
    [[nodiscard]] CellKind mutate_cell_kind(CellKind parent_kind, const MutationConfig& config);
    [[nodiscard]] float mutate_energy(float parent_energy, const MutationConfig& config);
    
private:
    std::mt19937 rng_;
};

} // namespace hex_engine
