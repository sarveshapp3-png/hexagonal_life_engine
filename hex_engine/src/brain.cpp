#include "hex_engine/brain.h"

namespace hex_engine {

Action SimpleBrain::decide(const Cell& self, const std::vector<VisionHit>& vision) {
    // Basic logic:
    // 1. If energy is high, reproduce.
    // 2. If food is seen, move towards it.
    // 3. Otherwise, just move forward or rotate randomly.
    
    if (self.energy > 15.0f) {
        return {ActionKind::Reproduce};
    }
    
    for (const auto& hit : vision) {
        if (hit.kind == CellKind::Food) {
            // In a real implementation, we'd check if it's in front.
            // For now, if we see food, we try to move.
            return {ActionKind::MoveForward};
        }
    }
    
    // Default to moving forward if we have energy.
    if (self.energy > 2.0f) {
        return {ActionKind::MoveForward};
    }
    
    return {ActionKind::None};
}

} // namespace hex_engine
