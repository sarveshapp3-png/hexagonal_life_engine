#pragma once

#include <string>
#include <filesystem>
#include "hex_engine/world.h"

namespace hex_engine {

// Persistence handles saving and loading world state to/from JSON.
class Persistence {
public:
    // Save the current world and tick count to a JSON file.
    // Returns true on success.
    static bool save_world(const World& world, uint32_t tick, const std::filesystem::path& path);

    // Load world state and tick count from a JSON file.
    // Returns true on success.
    static bool load_world(World& world, uint32_t& tick, const std::filesystem::path& path);
};

} // namespace hex_engine
