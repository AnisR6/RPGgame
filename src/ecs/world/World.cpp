
#include "ecs/world/World.hpp"
#include "ecs/system/SystemContext.hpp"

#include <vector>

namespace Engine::ECS {
    // ------------------------------------------------------------
    // Entity Management
    // ------------------------------------------------------------

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

    // ------------------------------------------------------------
    // Reset
    // ------------------------------------------------------------

    void World::Clear() noexcept {
        componentRegistry.Clear();
        entityManager.Clear();
    }

    // ------------------------------------------------------------
    // System Management
    // ------------------------------------------------------------

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

    // ------------------------------------------------------------
    // Private Helpers
    // ------------------------------------------------------------

    bool World::CanAccess(Entity entity) const noexcept {
        return entityManager.IsAlive(entity) && !entityManager.IsPendingDestroy(entity);
    }

} // namespace Engine::ECS
