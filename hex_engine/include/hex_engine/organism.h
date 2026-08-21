#pragma once

#include <cstdint>
#include <vector>
#include <unordered_map>
#include <memory>
#include <string>

#include "hex_engine/hex_coord.h"
#include "hex_engine/cell_kind.h"

namespace hex_engine {

class World;

struct AnatomyCell {
    HexCoord local_pos;
    CellKind kind;
};

struct Synapse {
    int source_node; // Input or Hidden
    int target_node; // Hidden or Output
    float weight;
};

struct NeuralNet {
    static constexpr int kInputCount = 8;
    static constexpr int kOutputCount = 5;
    static constexpr int kMaxHiddenNodes = 8;

    std::vector<Synapse> synapses;
    
    void randomize();
    void mutate(float rate);
    
    // Process inputs and return outputs
    std::vector<float> process(const std::vector<float>& inputs) const;
};

struct Genome {
    std::vector<AnatomyCell> anatomy;
    NeuralNet brain;
    
    void mutate(float rate);
    [[nodiscard]] float distance(const Genome& other) const;
    [[nodiscard]] static Genome crossover(const Genome& p1, const Genome& p2);
};

class Organism {
public:
    Organism(HexCoord pos, std::shared_ptr<Genome> genome, std::shared_ptr<class Species> species);
    
    HexCoord position;
    HexDirection rotation = HexDirection::East;
    std::shared_ptr<Genome> genome;
    std::shared_ptr<class Species> species;
    
    float energy = 0.0f;
    uint32_t age = 0;
    int health = 0;
    
    void update(World& world, const struct SimulationConfig& config, class OrganismRegistry& registry);
    bool can_reproduce(const struct SimulationConfig& config) const;
    std::shared_ptr<Organism> reproduce(World& world, const struct SimulationConfig& config);
    
    void take_damage(int amount, const struct SimulationConfig& config);
    
    [[nodiscard]] HexCoord get_world_pos(HexCoord local_pos) const;
    void sync_to_world(World& world) const;
    void clear_from_world(World& world) const;
};

class OrganismRegistry {
public:
    void add_organism(std::shared_ptr<Organism> organism);
    void update_all(World& world, const struct SimulationConfig& config);
    void clear();
    
    [[nodiscard]] std::vector<std::shared_ptr<Organism>>& organisms() { return organisms_; }
    [[nodiscard]] const std::vector<std::shared_ptr<Organism>>& organisms() const { return organisms_; }
    
    std::shared_ptr<Organism> get_organism_at(HexCoord world_pos) const;

private:
    std::vector<std::shared_ptr<Organism>> organisms_;
    std::unordered_map<HexCoord, std::shared_ptr<Organism>, HexCoordHash> pos_to_org_;
};

} // namespace hex_engine
