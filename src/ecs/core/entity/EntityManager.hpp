#pragma once

#include "ecs/core/entity/Entity.hpp"

#include <cstddef>
#include <vector>

namespace Engine::ECS {
    class EntityManager {
        public:
            EntityManager() = default;

            EntityManager(const EntityManager&) = delete;
            EntityManager& operator=(const EntityManager&) = delete;

            EntityManager(EntityManager&&) = delete;
            EntityManager& operator=(EntityManager&&) = delete;

            [[nodiscard]]
            Entity Create();

            [[nodiscard]]
            bool IsAlive(Entity entity) const noexcept;

            [[nodiscard]]
            Entity GetEntity(Entity::Index entityIndex) const noexcept;

            [[nodiscard]]
            bool IsPendingDestroy(Entity entity) const noexcept;

            [[nodiscard]]
            bool RequestDestroy(Entity entity);

            [[nodiscard]]
            std::vector<Entity> TakePendingDestruction() noexcept;

            void FlushDestruction(const std::vector<Entity>& entitiesToDestroy) noexcept;

            [[nodiscard]]
            std::size_t GetAliveEntityCount() const noexcept;

            void Clear() noexcept;

        private:
            struct EntitySlot {
                    Entity::Generation generation = 0;
                    Entity::Index nextFree = Entity::InvalidIndex;

                    bool isAlive = false;
                    bool isPendingDestroy = false;
            };

            Entity ReuseSlot();
            Entity CreateSlot();
            void AddSlot();
            Entity::Index RemoveNextFreeSlot() noexcept;
            void InitializeSlot(EntitySlot& slot) noexcept;

            bool IsEntitySlotInRange(Entity entity) const noexcept;

            void Destroy(Entity entity) noexcept;
            void FreeSlot(EntitySlot& slot) noexcept;
            bool IsGenerationExhausted(const EntitySlot& slot) const noexcept;
            void InvalidateSlot(EntitySlot& slot) noexcept;
            void RecycleSlot(Entity::Index index) noexcept;

            std::vector<EntitySlot> slots;
            std::vector<Entity> pendingDestroy;
            Entity::Index freeListHead = Entity::InvalidIndex;
            std::size_t aliveEntityCount = 0;
    };
}
