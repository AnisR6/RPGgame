#pragma once

#include "Entity.hpp"

#include <cstddef>
#include <vector>

namespace Engine::ECS {
    class EntityManager {
        public:
            // ------------------------------------------------------------
            // Construction
            // ------------------------------------------------------------

            EntityManager() = default;

            EntityManager(const EntityManager&) = delete;
            EntityManager& operator=(const EntityManager&) = delete;

            EntityManager(EntityManager&&) = delete;
            EntityManager& operator=(EntityManager&&) = delete;

            // ------------------------------------------------------------
            // Create
            // ------------------------------------------------------------

            [[nodiscard]]
            Entity Create();

            // ------------------------------------------------------------
            // Entity Access
            // ------------------------------------------------------------

            [[nodiscard]]
            bool IsAlive(Entity entity) const noexcept;

            [[nodiscard]]
            Entity GetEntity(Entity::Index entityIndex) const noexcept;

            // ------------------------------------------------------------
            // Pending Destruction
            // ------------------------------------------------------------

            [[nodiscard]]
            bool IsPendingDestroy(Entity entity) const noexcept;

            [[nodiscard]]
            bool RequestDestroy(Entity entity);

            [[nodiscard]]
            std::vector<Entity> TakePendingDestruction() noexcept;

            // ------------------------------------------------------------
            // Flush Destruction
            // ------------------------------------------------------------

            void FlushDestruction(const std::vector<Entity>& entitiesToDestroy) noexcept;

            // ------------------------------------------------------------
            // Entity Count
            // ------------------------------------------------------------

            [[nodiscard]]
            std::size_t GetAliveEntityCount() const noexcept;

            // ------------------------------------------------------------
            // Reset
            // ------------------------------------------------------------

            void Clear() noexcept;

        private:
            // ------------------------------------------------------------
            // EntitySlot
            // ------------------------------------------------------------

            struct EntitySlot {
                    Entity::Generation generation = 0;
                    Entity::Index nextFree = Entity::InvalidIndex;

                    bool isAlive = false;
                    bool isPendingDestroy = false;
            };

            // ------------------------------------------------------------
            // Create Helpers
            // ------------------------------------------------------------

            Entity ReuseSlot();
            Entity CreateSlot();
            void AddSlot();
            Entity::Index RemoveNextFreeSlot() noexcept;
            void InitializeSlot(EntitySlot& slot) noexcept;

            // ------------------------------------------------------------
            // Entity Validation
            // ------------------------------------------------------------

            bool IsEntitySlotInRange(Entity entity) const noexcept;

            // ------------------------------------------------------------
            // Destroy
            // ------------------------------------------------------------

            void Destroy(Entity entity) noexcept;
            void FreeSlot(EntitySlot& slot) noexcept;
            bool IsGenerationExhausted(const EntitySlot& slot) const noexcept;
            void InvalidateSlot(EntitySlot& slot) noexcept;
            void RecycleSlot(Entity::Index index) noexcept;

            // ------------------------------------------------------------
            // Storage
            // ------------------------------------------------------------

            std::vector<EntitySlot> slots;
            std::vector<Entity> pendingDestroy;
            Entity::Index freeListHead = Entity::InvalidIndex;
            std::size_t aliveEntityCount = 0;
    };

} // namespace Engine::ECS