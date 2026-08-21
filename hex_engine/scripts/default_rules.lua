-- default_rules.lua
-- Hexagonal Life Engine Configuration

return {
    -- Engine Settings
    target_fps = 60,
    max_ticks_per_frame = 1,
    wrap_world = true,
    
    -- Metabolism Rules
    producer_energy_per_tick = 0.6,    -- Increased from 0.5
    mover_energy_cost_per_step = 0.2,  -- Decreased from 0.3
    mouth_harvest_efficiency = 0.9,    -- Increased from 0.8
    base_energy_decay = 0.05,          -- Decreased from 0.1
    
    -- Reproduction Rules
    reproduction_threshold = 8.0,      -- Decreased from 10.0
    offspring_energy = 3.0,            -- Increased from 2.0
    parent_energy_cost = 4.0,          -- Increased from 3.0
    
    -- Evolution Rules
    mutation_rate = 0.1,
    mutation_add_prob = 0.33,
    mutation_remove_prob = 0.33,
    mutation_change_prob = 0.34,
    mutation_global_mutability = 0.05,

    -- Environment
    food_spawn_prob = 0.05,
    food_spawn_energy = 10.0,
    poison_damage = 10.0,
    
    -- Hazards
    virus_infection_chance = 0.05,
    explosive_radius = 2,
    explosive_damage = 50.0,
    
    -- Evolution (Advanced)
    sexual_reproduction = true,
    mate_search_radius = 3.0,
    kinship_threshold = 5.0,
    
    -- Disasters
    disaster_chance = 0.005,
    disaster_duration = 50
}
