#include "hex_engine/scripting.h"
#include <lua.hpp>
#include <iostream>

namespace hex_engine {

struct Scripting::LuaState {
    lua_State* L;
};

Scripting::Scripting() : state_(new LuaState) {
    state_->L = luaL_newstate();
    luaL_openlibs(state_->L);
}

Scripting::~Scripting() {
    lua_close(state_->L);
    delete state_;
}

bool Scripting::load_config(SimulationConfig& config, const std::filesystem::path& path) {
    if (luaL_dofile(state_->L, path.string().c_str()) != LUA_OK) {
        std::cerr << "Lua Error: " << lua_tostring(state_->L, -1) << std::endl;
        lua_pop(state_->L, 1);
        return false;
    }

    if (!lua_istable(state_->L, -1)) {
        std::cerr << "Lua Error: Script did not return a configuration table" << std::endl;
        lua_pop(state_->L, 1);
        return false;
    }

    auto get_float = [&](const char* key, float& val) {
        lua_getfield(state_->L, -1, key);
        if (lua_isnumber(state_->L, -1)) {
            val = static_cast<float>(lua_tonumber(state_->L, -1));
        }
        lua_pop(state_->L, 1);
    };

    auto get_uint = [&](const char* key, uint32_t& val) {
        lua_getfield(state_->L, -1, key);
        if (lua_isnumber(state_->L, -1)) {
            val = static_cast<uint32_t>(lua_tointeger(state_->L, -1));
        }
        lua_pop(state_->L, 1);
    };

    auto get_int = [&](const char* key, int& val) {
        lua_getfield(state_->L, -1, key);
        if (lua_isnumber(state_->L, -1)) {
            val = static_cast<int>(lua_tointeger(state_->L, -1));
        }
        lua_pop(state_->L, 1);
    };

    auto get_bool = [&](const char* key, bool& val) {
        lua_getfield(state_->L, -1, key);
        if (lua_isboolean(state_->L, -1)) {
            val = lua_toboolean(state_->L, -1);
        }
        lua_pop(state_->L, 1);
    };

    get_uint("target_fps", config.target_fps);
    get_uint("max_ticks_per_frame", config.max_ticks_per_frame);
    get_bool("wrap_world", config.wrap_world);

    get_float("producer_energy_per_tick", config.producer_energy_per_tick);
    get_float("mover_energy_cost_per_step", config.mover_energy_cost_per_step);
    get_float("mouth_harvest_efficiency", config.mouth_harvest_efficiency);
    get_float("base_energy_decay", config.base_energy_decay);

    get_float("reproduction_threshold", config.reproduction_threshold);
    get_float("offspring_energy", config.offspring_energy);
    get_float("parent_energy_cost", config.parent_energy_cost);

    get_float("mutation_rate", config.mutation_rate);
    get_float("mutation_add_prob", config.mutation_add_prob);
    get_float("mutation_remove_prob", config.mutation_remove_prob);
    get_float("mutation_change_prob", config.mutation_change_prob);
    get_float("mutation_global_mutability", config.mutation_global_mutability);

    get_bool("insta_kill", config.insta_kill);
    get_int("lifespan_multiplier", config.lifespan_multiplier);
    get_int("vision_range", config.vision_range);

    get_float("temperature_gradient", config.temperature_gradient);
    get_float("ideal_temperature", config.ideal_temperature);

    get_float("virus_infection_chance", config.virus_infection_chance);
    get_int("explosive_radius", config.explosive_radius);
    get_float("explosive_damage", config.explosive_damage);

    get_float("food_spawn_prob", config.food_spawn_prob);
    get_float("food_spawn_energy", config.food_spawn_energy);
    get_float("poison_damage", config.poison_damage);

    get_bool("sexual_reproduction", config.sexual_reproduction);
    get_float("mate_search_radius", config.mate_search_radius);
    get_float("kinship_threshold", config.kinship_threshold);

    get_float("disaster_chance", config.disaster_chance);
    get_int("disaster_duration", config.disaster_duration);

    lua_pop(state_->L, 1); // Pop the table
    return true;
}

} // namespace hex_engine
