#pragma once

#include "ecs/core/entity/Entity.hpp"

#include <cstddef>

namespace Engine::ECS {
    using ComponentTypeId = std::size_t;

    class IComponentPool {
        public:
            IComponentPool() = default;
            virtual ~IComponentPool() = default;

            IComponentPool(const IComponentPool&) = delete;
            IComponentPool& operator=(const IComponentPool&) = delete;

            IComponentPool(IComponentPool&&) = delete;
            IComponentPool& operator=(IComponentPool&&) = delete;

            virtual bool RemoveComponent(Entity::Index entityIndex) noexcept = 0;

            [[nodiscard]]
            virtual std::size_t GetSize() const noexcept = 0;

            [[nodiscard]]
            virtual ComponentTypeId GetComponentPoolTypeId() const noexcept = 0;
    };
}
