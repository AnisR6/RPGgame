#pragma once

#include <cstdint>
#include <limits>

namespace Engine::ECS {
    class Entity {
        public:
            using Index = std::uint32_t;
            using Generation = std::uint32_t;

            static constexpr Index InvalidIndex = std::numeric_limits<Index>::max();
            static constexpr Generation InvalidGeneration = std::numeric_limits<Generation>::max();

            constexpr Entity() noexcept = default;

            constexpr Entity(Index index, Generation generation) noexcept
                : index(index),
                  generation(generation) {}

            [[nodiscard]]
            constexpr Index GetIndex() const noexcept {
                return index;
            }

            [[nodiscard]]
            constexpr Generation GetGeneration() const noexcept {
                return generation;
            }

            [[nodiscard]]
            constexpr bool IsValid() const noexcept {
                return index != InvalidIndex && generation != InvalidGeneration;
            }

            constexpr bool operator==(const Entity&) const noexcept = default;

        private:
            Index index = InvalidIndex;
            Generation generation = InvalidGeneration;
    };
}
