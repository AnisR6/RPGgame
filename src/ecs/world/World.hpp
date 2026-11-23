
#pragma once

#include "ecs/entity/Entity.hpp"
#include "ecs/entity/EntityManager.hpp"
#include "ecs/storage/ComponentRegistry.hpp"
#include "ecs/system/SystemManager.hpp"

#include <stdexcept>
#include <utility>

namespace Engine::ECS {
    // ------------------------------------------------------------
    // World
    // ------------------------------------------------------------

    class World {
        public:
            // ------------------------------------------------------------
            // Construction
            // ------------------------------------------------------------

            World() = default;

            World(const World&) = delete;
            World& operator=(const World&) = delete;

            World(World&&) = delete;
            World& operator=(World&&) = delete;

            ~World() = default;

            // ------------------------------------------------------------
            // Entity Management
            // ------------------------------------------------------------

            [[nodiscard]]
            Entity CreateEntity();

            [[nodiscard]]
            bool IsAlive(Entity entity) const noexcept;

            [[nodiscard]]
            bool IsPendingDestroy(Entity entity) const noexcept;

            [[nodiscard]]
            bool DestroyEntity(Entity entity);

            void FlushStructuralChanges();

            // ------------------------------------------------------------
            // Reset
            // ------------------------------------------------------------

            void Clear() noexcept;

            // ------------------------------------------------------------
            // Component Management
            // ------------------------------------------------------------

            template <typename T, typename... Args>
            T& EmplaceComponent(Entity entity, Args&&... args);

            template <typename T> T& GetComponent(Entity entity) const;

            template <typename T>
            [[nodiscard]]
            bool HasComponent(Entity entity) const;

            template <typename T>
            [[nodiscard]]
            bool RemoveComponent(Entity entity);

            // ------------------------------------------------------------
            // Query Creation
            // ------------------------------------------------------------

            template <typename IncludeSet, typename ExcludeSet = Excluding<>>
            [[nodiscard]]
            Query<IncludeSet, ExcludeSet> CreateQuery() {
                return Query<IncludeSet, ExcludeSet>(entityManager, componentRegistry);
            }

            // ------------------------------------------------------------
            // System Management
            // ------------------------------------------------------------

            void UpdateSystems(SystemManager& systemManager, float deltaTime);

        private:
            // ------------------------------------------------------------
            // Entity Access Validation
            // ------------------------------------------------------------

            [[nodiscard]]
            bool CanAccess(Entity entity) const noexcept;

            // ------------------------------------------------------------
            // Storage
            // ------------------------------------------------------------

            EntityManager entityManager;
            ComponentRegistry componentRegistry;
    };

    // ------------------------------------------------------------
    // EmplaceComponent
    // ------------------------------------------------------------

    template <typename T, typename... Args>
    T& World::EmplaceComponent(Entity entity, Args&&... args) {
        if (!CanAccess(entity))
            throw std::invalid_argument("World: Cannot add a component to an inaccessible entity.");

        return componentRegistry.GetOrCreatePool<T>().EmplaceComponent(entity.GetIndex(),
                                                                       std::forward<Args>(args)...);
    }

    // ------------------------------------------------------------
    // GetComponent
    // ------------------------------------------------------------

    template <typename T> T& World::GetComponent(Entity entity) const {
        if (!CanAccess(entity))
            throw std::invalid_argument("World: Cannot access an inaccessible entity.");

        ComponentPool<T>* componentPool = componentRegistry.GetPool<T>();

        if (componentPool == nullptr)
            throw std::out_of_range("World: Component pool does not exist.");

        return componentPool->GetComponent(entity.GetIndex());
    }

    // ------------------------------------------------------------
    // HasComponent
    // ------------------------------------------------------------

    template <typename T> bool World::HasComponent(Entity entity) const {
        if (!CanAccess(entity))
            return false;

        ComponentPool<T>* componentPool = componentRegistry.GetPool<T>();

        return componentPool != nullptr && componentPool->ContainsComponent(entity.GetIndex());
    }

    // ------------------------------------------------------------
    // RemoveComponent
    // ------------------------------------------------------------

    template <typename T> bool World::RemoveComponent(Entity entity) {
        if (!CanAccess(entity))
            return false;

        ComponentPool<T>* componentPool = componentRegistry.GetPool<T>();

        return componentPool != nullptr && componentPool->RemoveComponent(entity.GetIndex());
    }

} // namespace Engine::ECS
