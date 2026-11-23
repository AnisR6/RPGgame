#pragma once

#include "ecs/core/entity/Entity.hpp"
#include "ecs/core/entity/EntityManager.hpp"
#include "ecs/core/query/Query.hpp"
#include "ecs/core/storage/ComponentRegistry.hpp"

#include <stdexcept>
#include <utility>

namespace Engine::ECS {
    class SystemContext {
        public:
            SystemContext(const SystemContext&) = delete;
            SystemContext& operator=(const SystemContext&) = delete;

            SystemContext(SystemContext&&) = delete;
            SystemContext& operator=(SystemContext&&) = delete;

            [[nodiscard]]
            Entity CreateEntity() {
                return entityManager.Create();
            }

            [[nodiscard]]
            bool IsAlive(Entity entity) const noexcept {
                return entityManager.IsAlive(entity);
            }

            [[nodiscard]]
            bool IsPendingDestroy(Entity entity) const noexcept {
                return entityManager.IsPendingDestroy(entity);
            }

            [[nodiscard]]
            bool DestroyEntity(Entity entity) {
                return entityManager.RequestDestroy(entity);
            }

            template <typename T, typename... Args>
            T& EmplaceComponent(Entity entity, Args&&... args) {
                if (!CanAccess(entity))
                    throw std::invalid_argument(
                        "SystemContext: Cannot add a component to an inaccessible entity.");

                return componentRegistry.GetOrCreatePool<T>().EmplaceComponent(
                    entity.GetIndex(), std::forward<Args>(args)...);
            }

            template <typename T> T& GetComponent(Entity entity) const {
                if (!CanAccess(entity))
                    throw std::invalid_argument(
                        "SystemContext: Cannot access an inaccessible entity.");

                ComponentPool<T>* componentPool = componentRegistry.GetPool<T>();

                if (componentPool == nullptr)
                    throw std::out_of_range("SystemContext: Component pool does not exist.");

                return componentPool->GetComponent(entity.GetIndex());
            }

            template <typename T>
            [[nodiscard]]
            bool HasComponent(Entity entity) const {
                if (!CanAccess(entity))
                    return false;

                ComponentPool<T>* componentPool = componentRegistry.GetPool<T>();

                return componentPool != nullptr &&
                       componentPool->ContainsComponent(entity.GetIndex());
            }

            template <typename T>
            [[nodiscard]]
            bool RemoveComponent(Entity entity) {
                if (!CanAccess(entity))
                    return false;

                ComponentPool<T>* componentPool = componentRegistry.GetPool<T>();

                return componentPool != nullptr &&
                       componentPool->RemoveComponent(entity.GetIndex());
            }

            template <typename IncludeSet, typename ExcludeSet = Excluding<>>
            [[nodiscard]]
            Query<IncludeSet, ExcludeSet> CreateQuery() {
                return Query<IncludeSet, ExcludeSet>(entityManager, componentRegistry);
            }

        private:
            friend class World;

            SystemContext(EntityManager& entityManager, ComponentRegistry& componentRegistry)
                : entityManager(entityManager),
                  componentRegistry(componentRegistry) {}

            [[nodiscard]]
            bool CanAccess(Entity entity) const noexcept {
                return entityManager.IsAlive(entity) && !entityManager.IsPendingDestroy(entity);
            }

            EntityManager& entityManager;
            ComponentRegistry& componentRegistry;
    };
}
