#include "hex_engine/species.h"
#include "hex_engine/organism.h"
#include <sstream>
#include <iomanip>

namespace hex_engine {

Species::Species(std::string n, std::shared_ptr<Genome> g, uint32_t tick, std::string a_name)
    : name(n), ancestor_name(a_name), genome(g), start_tick(tick) {
    for (const auto& ac : genome->anatomy) {
        cell_counts[ac.kind]++;
    }
}

void Species::add_instance() {
    population++;
    cumulative_pop++;
}

void Species::remove_instance() {
    if (population > 0) population--;
}

std::shared_ptr<Species> FossilRecord::record_new_species(std::shared_ptr<Genome> genome, std::shared_ptr<Species> ancestor, uint32_t tick) {
    std::string name = generate_name();
    std::string ancestor_name = ancestor ? ancestor->name : "";
    auto species = std::make_shared<Species>(name, genome, tick, ancestor_name);
    extant_species_[name] = species;
    return species;
}

void FossilRecord::update_stats(uint32_t tick, uint32_t total_orgs) {
    (void)total_orgs;
    for (auto it = extant_species_.begin(); it != extant_species_.end(); ) {
        if (it->second->population == 0) {
            it->second->end_tick = tick;
            extinct_species_.push_back(it->second);
            it = extant_species_.erase(it);
        } else {
            ++it;
        }
    }
}

std::string FossilRecord::export_lineage() const {
    std::stringstream ss;
    ss << "digraph Lineage {\n";
    for (const auto& [name, s] : extant_species_) {
        if (!s->ancestor_name.empty()) {
            ss << "  " << s->ancestor_name << " -> " << s->name << ";\n";
        }
    }
    for (const auto& s : extinct_species_) {
        if (!s->ancestor_name.empty()) {
            ss << "  " << s->ancestor_name << " -> " << s->name << ";\n";
        }
    }
    ss << "}\n";
    return ss.str();
}

std::string FossilRecord::generate_name() {
    std::stringstream ss;
    ss << "Species_" << std::setw(4) << std::setfill('0') << ++species_counter_;
    return ss.str();
}

} // namespace hex_engine
