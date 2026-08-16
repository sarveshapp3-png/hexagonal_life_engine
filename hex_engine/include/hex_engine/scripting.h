#pragma once

#include <string>
#include <filesystem>
#include "hex_engine/simulation.h"

namespace hex_engine {

// Scripting handles loading configuration and rules from Lua scripts.
class Scripting {
public:
    Scripting();
    ~Scripting();

    // Load simulation configuration from a Lua file.
    // Returns true on success.
    bool load_config(SimulationConfig& config, const std::filesystem::path& path);

private:
    struct LuaState;
    LuaState* state_;
};

} // namespace hex_engine
