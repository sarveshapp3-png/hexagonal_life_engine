#include "hex_engine/persistence.h"
#include <fstream>
#include <nlohmann/json.hpp>
#include "hex_engine/cell_kind.h"

namespace hex_engine {

using json = nlohmann::json;

bool Persistence::save_world(const World& world, uint32_t tick, const std::filesystem::path& path) {
    json root;
    root["version"] = 1;
    root["tick"] = tick;
    
    json cells_array = json::array();
    for (const auto& coord : world.occupied_coords()) {
        const Cell* cell = world.find_cell(coord);
        if (cell) {
            json cell_obj;
            cell_obj["q"] = coord.q;
            cell_obj["r"] = coord.r;
            cell_obj["kind"] = cell_kind_name(cell->kind);
            cell_obj["energy"] = cell->energy;
            cell_obj["age"] = cell->age;
            cell_obj["facing"] = hex_direction_name(cell->facing);
            cells_array.push_back(cell_obj);
        }
    }
    root["cells"] = cells_array;

    std::ofstream file(path);
    if (!file.is_open()) return false;
    
    file << root.dump(4);
    return true;
}

bool Persistence::load_world(World& world, uint32_t& tick, const std::filesystem::path& path) {
    std::ifstream file(path);
    if (!file.is_open()) return false;

    json root;
    try {
        file >> root;
    } catch (const json::parse_error&) {
        return false;
    }

    if (!root.contains("cells") || !root["cells"].is_array()) return false;

    world.clear();
    tick = root.value("tick", 0U);

    for (const auto& cell_json : root["cells"]) {
        HexCoord coord{ cell_json.value("q", 0), cell_json.value("r", 0) };
        Cell cell;
        cell.kind = cell_kind_from_name(cell_json.value("kind", "Empty"));
        cell.energy = cell_json.value("energy", 1.0f);
        cell.age = cell_json.value("age", 0U);
        cell.facing = hex_direction_from_name(cell_json.value("facing", "East"));
        
        world.set_cell_full(coord, cell);
    }

    return true;
}

} // namespace hex_engine
