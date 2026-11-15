
#pragma once

#include "game_state/ecs/entity/Entity.hpp"

#include <cstddef>

namespace Engine::ECS {
    using ComponentTypeId = std::size_t;

    class IComponentPool {
        public:
            // ------------------------------------------------------------
            // Construction
            // ------------------------------------------------------------

            IComponentPool() = default;
            virtual ~IComponentPool() = default;

            IComponentPool(const IComponentPool&) = delete;
            IComponentPool& operator=(const IComponentPool&) = delete;

            IComponentPool(IComponentPool&&) = delete;
            IComponentPool& operator=(IComponentPool&&) = delete;

            // ------------------------------------------------------------
            // Component Management
            // ------------------------------------------------------------

            virtual bool RemoveComponent(Entity::Index entityIndex) noexcept = 0;

            // ------------------------------------------------------------
            // Accessors
            // ------------------------------------------------------------

            [[nodiscard]]
            virtual std::size_t GetSize() const noexcept = 0;

            [[nodiscard]]
            virtual ComponentTypeId GetComponentPoolTypeId() const noexcept = 0;
    };

} // namespace Engine::ECS
