#pragma once

#include "ecs/core/entity/Entity.hpp"

#include <cstddef>
#include <limits>
#include <vector>

namespace Engine::ECS {
    class SparseSet {
        public:
            using Index = Entity::Index;

            static constexpr Index InvalidIndex = std::numeric_limits<Index>::max();

            [[nodiscard]]
            bool Contains(Index entityIndex) const noexcept;

            bool Insert(Index entityIndex);

            bool Remove(Index entityIndex) noexcept;

            void Reserve(std::size_t capacity);

            [[nodiscard]]
            Index GetPackedIndex(Index entityIndex) const noexcept;

            [[nodiscard]]
            Index GetPackedEntity(std::size_t packedIndex) const noexcept;

            [[nodiscard]]
            const std::vector<Index>& GetPackedEntities() const noexcept;

            [[nodiscard]]
            std::size_t GetSize() const noexcept;

            [[nodiscard]]
            bool IsEmpty() const noexcept;

        private:
            std::vector<Index> sparse;
            std::vector<Index> packed;
    };
}
