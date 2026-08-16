#pragma once

#include <vector>

#include "hex_engine/hex_coord.h"
#include "hex_engine/vision.h"

namespace hex_engine {

// Actions a cell can take during a tick.
enum class ActionKind {
    None,
    MoveForward,
    RotateClockwise,
    RotateCounterClockwise,
    Reproduce,
    Eat
};

struct Action {
    ActionKind kind = ActionKind::None;
};

// A simple placeholder for neural network or rule-based brains.
class Brain {
public:
    virtual ~Brain() = default;
    
    // Decide an action based on current state and vision.
    [[nodiscard]] virtual Action decide(const Cell& self, const std::vector<VisionHit>& vision) = 0;
};

// A basic rule-based brain for testing.
class SimpleBrain final : public Brain {
public:
    [[nodiscard]] Action decide(const Cell& self, const std::vector<VisionHit>& vision) override;
};

} // namespace hex_engine
