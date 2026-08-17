#pragma once

#include <string>
#include <vector>
#include <unordered_map>
#include <memory>
#include "hex_engine/cell_kind.h"

namespace hex_engine {

struct Genome;

class Species {
public:
    Species(std::string name, std::shared_ptr<Genome> founding_genome, uint32_t start_tick);
    
    std::string name;
    std::shared_ptr<Genome> genome;
    uint32_t start_tick;
    uint32_t end_tick = 0;
    
    uint32_t population = 0;
    uint32_t cumulative_pop = 0;
    
    std::unordered_map<CellKind, uint32_t> cell_counts;
    
    void add_instance();
    void remove_instance();
};

class FossilRecord {
public:
    FossilRecord() = default;
    
    std::shared_ptr<Species> record_new_species(std::shared_ptr<Genome> genome, std::shared_ptr<Species> ancestor, uint32_t tick);
    void update_stats(uint32_t tick, uint32_t total_orgs);
    
    const std::unordered_map<std::string, std::shared_ptr<Species>>& extant_species() const { return extant_species_; }
    
private:
    std::unordered_map<std::string, std::shared_ptr<Species>> extant_species_;
    std::vector<std::shared_ptr<Species>> extinct_species_;
    uint32_t species_counter_ = 0;
    
    std::string generate_name();
};

} // namespace hex_engine
