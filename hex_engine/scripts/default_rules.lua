-- This file is intentionally tiny.
-- It exists so the project starts data-driven instead of hardcoding rules into
-- the native code from day one.

return {
  simulation = {
    target_fps = 60,          -- The first milestone target for the future loop.
    wrap_world = true,        -- Wraparound is a safe default for infinite-feel maps.
    max_ticks_per_frame = 1,  -- Deterministic step control comes first.
  },
  hex = {
    orientation = "pointy",   -- We are building around axial coordinates first.
    direction_order = {
      "east",
      "northeast",
      "northwest",
      "west",
      "southwest",
      "southeast",
    },
  },
  palette = {
    empty = "#081425",        -- Deep background from the stitched design language.
    food = "#5b6b7a",         -- Neutral resource tint for now.
    wall = "#2f3a4c",         -- Structural blocker.
    mover = "#22c55e",        -- Vitality / motion.
    producer = "#22d3ee",     -- Utility / growth.
    mouth = "#f59e0b",        -- Harvest / consumption.
    killer = "#ef4444",       -- Aggression / combat.
    armor = "#a855f7",        -- Defense / durability.
    eye = "#c084fc",          -- Vision / cognition.
  },
}

