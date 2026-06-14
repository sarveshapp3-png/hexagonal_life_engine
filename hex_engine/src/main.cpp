#include <array>
#include <iostream>
#include <string_view>

#include "hex_engine/cell_kind.h"
#include "hex_engine/hex_coord.h"
#include "hex_engine/world.h"

namespace hex_engine_demo {

// The official Life Engine title mark is kept as a reference asset so the
// future OpenGL shell can reuse the same branding file instead of inventing a
// second logo path later.
constexpr std::string_view kReferenceLogoPath = "assets/reference/life-engine-title.png";

// Direction labels are kept local to the demo so the engine core does not need
// to care about presentation-layer text yet.
[[nodiscard]] constexpr std::string_view hex_direction_name(const hex_engine::HexDirection direction) noexcept {
    switch (direction) {
    case hex_engine::HexDirection::East:
        return "East";
    case hex_engine::HexDirection::NorthEast:
        return "NorthEast";
    case hex_engine::HexDirection::NorthWest:
        return "NorthWest";
    case hex_engine::HexDirection::West:
        return "West";
    case hex_engine::HexDirection::SouthWest:
        return "SouthWest";
    case hex_engine::HexDirection::SouthEast:
        return "SouthEast";
    }

    return "Unknown";
}

// This helper keeps all report formatting in one place so future UI and logger
// code can reuse the same deterministic world snapshot logic.
void print_world_report(const hex_engine::World& world) {
    using namespace hex_engine;

    std::cout << "Hex Engine Foundation Build\n";
    std::cout << "Occupied cells: " << world.occupied_count() << "\n";

    const WorldBounds bounds = compute_bounds(world);
    if (bounds.valid) {
        std::cout << "Bounds min: " << bounds.min << "\n";
        std::cout << "Bounds max: " << bounds.max << "\n";
    } else {
        std::cout << "Bounds: empty world\n";
    }

    std::cout << "\nSeeded cells:\n";
    for (const HexCoord coord : world.occupied_coords()) {
        std::cout << "  - " << coord
                  << " kind=" << cell_kind_name(world.kind_at(coord))
                  << " dist_from_origin=" << hex_distance(coord, HexCoord{}) << "\n";
    }
}

void print_neighbor_table() {
    using namespace hex_engine;

    const HexCoord origin{};
    std::cout << "\nOrigin neighbors:\n";

    for (std::size_t index = 0; index < kHexDirectionOffsets.size(); ++index) {
        const HexDirection direction = static_cast<HexDirection>(index);
        const HexCoord neighbor = hex_neighbor(origin, direction);
        std::cout << "  - " << hex_direction_name(direction) << ": " << neighbor << "\n";
    }
}

} // namespace hex_engine_demo

int main() {
    using namespace hex_engine;

    // The first demo seed is tiny on purpose. We want an obvious sanity check
    // rather than a noisy simulation before the real game loop exists.
    World world;

    const HexCoord origin{};

    world.set_cell(origin, CellKind::Mover);
    world.set_cell(hex_neighbor(origin, HexDirection::East), CellKind::Producer);
    world.set_cell(hex_neighbor(origin, HexDirection::NorthEast), CellKind::Food);
    world.set_cell(hex_neighbor(origin, HexDirection::NorthWest), CellKind::Wall);
    world.set_cell(hex_neighbor(origin, HexDirection::West), CellKind::Killer);
    world.set_cell(hex_neighbor(origin, HexDirection::SouthWest), CellKind::Armor);
    world.set_cell(hex_neighbor(origin, HexDirection::SouthEast), CellKind::Eye);
    world.set_cell(origin.translated(2, -1), CellKind::Mouth);

    hex_engine_demo::print_world_report(world);
    hex_engine_demo::print_neighbor_table();

    std::cout << "\nLua data will drive rules in the next step, and OpenGL will\n";
    std::cout << "sit on top once the simulation core is solid.\n";
    std::cout << "Reference logo asset: " << kReferenceLogoPath << "\n";
    std::cout << "The first OpenGL window will reuse that file as branding.\n";

    return 0;
}
