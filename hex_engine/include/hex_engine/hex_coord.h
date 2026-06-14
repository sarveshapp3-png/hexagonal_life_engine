#pragma once

#include <array>
#include <cstddef>
#include <cstdint>
#include <functional>
#include <ostream>

namespace hex_engine {

// Axial hex coordinates keep the math compact while still matching the
// symmetrical structure we need for a future pointy-top renderer.
struct HexCoord final {
    int q = 0; // Axial column coordinate.
    int r = 0; // Axial row coordinate.

    // The third cube coordinate is derived instead of stored, which keeps the
    // representation small and avoids duplicated state.
    [[nodiscard]] constexpr int s() const noexcept {
        return -q - r;
    }

    // A tiny helper for local offsets, which we use everywhere that needs a
    // deterministic translation in hex space.
    [[nodiscard]] constexpr HexCoord translated(const int dq, const int dr) const noexcept {
        return HexCoord{q + dq, r + dr};
    }

    // Equality is structural, so the default generated comparison is exactly
    // what we want.
    [[nodiscard]] constexpr bool operator==(const HexCoord&) const noexcept = default;
};

// Stable direction ordering matters because simulation logic, user input, and
// rendering overlays should all agree on the same six-way neighborhood.
enum class HexDirection : std::uint8_t {
    East = 0,
    NorthEast = 1,
    NorthWest = 2,
    West = 3,
    SouthWest = 4,
    SouthEast = 5,
};

// The axial direction vectors below follow the standard cube/axial pattern from
// established hex-grid practice, which keeps future algorithms simple.
inline constexpr std::array<HexCoord, 6> kHexDirectionOffsets = {{
    {+1, 0},   // East.
    {+1, -1},  // NorthEast.
    {0, -1},   // NorthWest.
    {-1, 0},   // West.
    {-1, +1},  // SouthWest.
    {0, +1},   // SouthEast.
}};

// A tiny integer absolute-value helper keeps the distance function constexpr and
// avoids dragging in floating-point math for the core grid logic.
[[nodiscard]] constexpr int hex_abs(const int value) noexcept {
    return value < 0 ? -value : value;
}

// Hex distance is one of the key primitives we will reuse for movement, vision,
// range queries, camera logic, and future AI behaviors.
[[nodiscard]] constexpr int hex_distance(const HexCoord a, const HexCoord b) noexcept {
    const int dq = a.q - b.q;
    const int dr = a.r - b.r;
    const int ds = a.s() - b.s();
    return (hex_abs(dq) + hex_abs(dr) + hex_abs(ds)) / 2;
}

// Single-step neighbor lookup is intentionally explicit so the direction table
// remains the one true source of movement truth.
[[nodiscard]] constexpr HexCoord hex_neighbor(const HexCoord coord, const HexDirection direction) noexcept {
    const std::size_t index = static_cast<std::size_t>(direction);
    return coord.translated(kHexDirectionOffsets[index].q, kHexDirectionOffsets[index].r);
}

// Streaming support makes the first console demo clean and keeps debugging easy.
inline std::ostream& operator<<(std::ostream& stream, const HexCoord coord) {
    stream << "(" << coord.q << ", " << coord.r << ", s=" << coord.s() << ")";
    return stream;
}

// Sparse world storage needs a stable hash so hex coordinates can live in hash
// maps without any extra adapter layer.
struct HexCoordHash final {
    [[nodiscard]] std::size_t operator()(const HexCoord coord) const noexcept {
        const std::size_t q_hash = std::hash<int>{}(coord.q);
        const std::size_t r_hash = std::hash<int>{}(coord.r);

        // This is a standard hash-combine style mix; it is simple, fast, and
        // good enough for a sparse simulation grid.
        return q_hash ^ (r_hash + 0x9e3779b97f4a7c15ULL + (q_hash << 6U) + (q_hash >> 2U));
    }
};

} // namespace hex_engine

