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
    mutation_rate = 0.15,              -- Increased from 0.1
    energy_variance = 0.25             -- Increased from 0.2
}
