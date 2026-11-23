#include "ecs/core/world/World.hpp"
#include "ecs/core/systems/SystemContext.hpp"

#include <vector>

namespace Engine::ECS {
    Entity World::CreateEntity() {
        return entityManager.Create();
    }

    bool World::IsAlive(Entity entity) const noexcept {
        return entityManager.IsAlive(entity);
    }

    bool World::IsPendingDestroy(Entity entity) const noexcept {
        return entityManager.IsPendingDestroy(entity);
    }

    bool World::DestroyEntity(Entity entity) {
        return entityManager.RequestDestroy(entity);
    }

    void World::FlushStructuralChanges() {
        const std::vector<Entity> entitiesToDestroy = entityManager.TakePendingDestruction();

        for (const Entity entity : entitiesToDestroy) {
            componentRegistry.RemoveComponents(entity.GetIndex());
        }

        entityManager.FlushDestruction(entitiesToDestroy);
    }

    void World::Clear() noexcept {
        componentRegistry.Clear();
        entityManager.Clear();
    }

    void World::UpdateSystems(SystemManager& systemManager, float deltaTime) {
        SystemContext context(entityManager, componentRegistry);

        try {
            systemManager.Update(context, deltaTime);
        } catch (...) {
            FlushStructuralChanges();
            throw;
        }

        FlushStructuralChanges();
    }

    bool World::CanAccess(Entity entity) const noexcept {
        return entityManager.IsAlive(entity) && !entityManager.IsPendingDestroy(entity);
    }
}
