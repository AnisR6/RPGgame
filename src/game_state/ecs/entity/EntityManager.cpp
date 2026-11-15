#include "game_state/ecs/entity/EntityManager.hpp"

#include <stdexcept>
#include <vector>

namespace Engine::ECS {

    // ------------------------------------------------------------
    // Entity Creation
    // ------------------------------------------------------------

    Entity EntityManager::Create() {
        if (freeListHead != Entity::InvalidIndex) {
            return ReuseSlot();
        }

        return CreateSlot();
    }

    // ------------------------------------------------------------
    // Slot Reuse
    // ------------------------------------------------------------

    Entity EntityManager::ReuseSlot() {
        const Entity::Index entityIndex = RemoveNextFreeSlot();
        EntitySlot& entitySlot = slots[entityIndex];

        InitializeSlot(entitySlot);

        return Entity{entityIndex, entitySlot.generation};
    }

    // ------------------------------------------------------------
    // Slot Creation
    // ------------------------------------------------------------

    Entity EntityManager::CreateSlot() {
        if (slots.size() >= Entity::InvalidIndex) {
            throw std::overflow_error("EntityManager.slots capacity reached.");
        }

        const Entity::Index entityIndex = static_cast<Entity::Index>(slots.size());

        AddSlot();

        EntitySlot& entitySlot = slots.back();
        InitializeSlot(entitySlot);

        return Entity{entityIndex, entitySlot.generation};
    }

    void EntityManager::AddSlot() {
        slots.emplace_back();
    }

    // ------------------------------------------------------------
    // Free List Management
    // ------------------------------------------------------------

    Entity::Index EntityManager::RemoveNextFreeSlot() noexcept {
        const Entity::Index entityIndex = freeListHead;
        EntitySlot& entitySlot = slots[entityIndex];

        freeListHead = entitySlot.nextFree;
        entitySlot.nextFree = Entity::InvalidIndex;

        return entityIndex;
    }

    // ------------------------------------------------------------
    // Slot Initialization
    // ------------------------------------------------------------

    void EntityManager::InitializeSlot(EntitySlot& slot) noexcept {
        slot.isAlive = true;
        slot.isPendingDestroy = false;
        slot.nextFree = Entity::InvalidIndex;

        ++aliveEntityCount;
    }

    // ------------------------------------------------------------
    // Entity Validation
    // ------------------------------------------------------------

    bool EntityManager::IsAlive(Entity entity) const noexcept {
        if (!entity.IsValid() || !IsEntitySlotInRange(entity)) {
            return false;
        }

        const EntitySlot& entitySlot = slots[entity.GetIndex()];

        return entitySlot.isAlive && entitySlot.generation == entity.GetGeneration();
    }

    bool EntityManager::IsEntitySlotInRange(Entity entity) const noexcept {
        return entity.GetIndex() < slots.size();
    }

    Entity EntityManager::GetEntity(Entity::Index entityIndex) const noexcept {
        if (entityIndex >= slots.size()) {
            return Entity{};
        }

        const EntitySlot& entitySlot = slots[entityIndex];

        if (!entitySlot.isAlive) {
            return Entity{};
        }

        return Entity{entityIndex, entitySlot.generation};
    }

    // ------------------------------------------------------------
    // Pending Destruction
    // ------------------------------------------------------------

    bool EntityManager::IsPendingDestroy(Entity entity) const noexcept {
        if (!IsAlive(entity)) {
            return false;
        }

        return slots[entity.GetIndex()].isPendingDestroy;
    }

    bool EntityManager::RequestDestroy(Entity entity) {
        if (!IsAlive(entity) || IsPendingDestroy(entity)) {
            return false;
        }

        pendingDestroy.push_back(entity);
        slots[entity.GetIndex()].isPendingDestroy = true;

        return true;
    }

    std::vector<Entity> EntityManager::TakePendingDestruction() noexcept {
        std::vector<Entity> entitiesToDestroy;
        entitiesToDestroy.swap(pendingDestroy);

        return entitiesToDestroy;
    }

    void EntityManager::FlushDestruction(const std::vector<Entity>& entitiesToDestroy) noexcept {
        for (Entity entity : entitiesToDestroy) {
            Destroy(entity);
        }
    }

    // ------------------------------------------------------------
    // Entity Destruction
    // ------------------------------------------------------------

    void EntityManager::Destroy(Entity entity) noexcept {
        if (!IsAlive(entity)) {
            return;
        }

        const Entity::Index entityIndex = entity.GetIndex();
        EntitySlot& entitySlot = slots[entityIndex];

        FreeSlot(entitySlot);

        if (IsGenerationExhausted(entitySlot)) {
            InvalidateSlot(entitySlot);
            return;
        }

        RecycleSlot(entityIndex);
    }

    void EntityManager::FreeSlot(EntitySlot& slot) noexcept {
        slot.isAlive = false;
        slot.isPendingDestroy = false;

        --aliveEntityCount;
    }

    // ------------------------------------------------------------
    // Generation Management
    // ------------------------------------------------------------

    bool EntityManager::IsGenerationExhausted(const EntitySlot& slot) const noexcept {
        return slot.generation == Entity::InvalidGeneration - 1;
    }

    void EntityManager::InvalidateSlot(EntitySlot& slot) noexcept {
        slot.generation = Entity::InvalidGeneration;
        slot.nextFree = Entity::InvalidIndex;
    }

    void EntityManager::RecycleSlot(Entity::Index entityIndex) noexcept {
        EntitySlot& entitySlot = slots[entityIndex];

        ++entitySlot.generation;

        entitySlot.nextFree = freeListHead;
        freeListHead = entityIndex;
    }

    // ------------------------------------------------------------
    // Entity Count
    // ------------------------------------------------------------

    std::size_t EntityManager::GetAliveEntityCount() const noexcept {
        return aliveEntityCount;
    }

    // ------------------------------------------------------------
    // Reset
    // ------------------------------------------------------------

    void EntityManager::Clear() noexcept {
        pendingDestroy.clear();

        freeListHead = Entity::InvalidIndex;
        aliveEntityCount = 0;

        for (std::size_t slotIndex = slots.size(); slotIndex > 0; --slotIndex) {
            EntitySlot& entitySlot = slots[slotIndex - 1];

            if (entitySlot.isAlive) {
                if (IsGenerationExhausted(entitySlot)) {
                    InvalidateSlot(entitySlot);
                } else {
                    ++entitySlot.generation;
                }
            }

            entitySlot.isAlive = false;
            entitySlot.isPendingDestroy = false;

            if (entitySlot.generation == Entity::InvalidGeneration) {
                entitySlot.nextFree = Entity::InvalidIndex;
                continue;
            }

            entitySlot.nextFree = freeListHead;
            freeListHead = static_cast<Entity::Index>(slotIndex - 1);
        }
    }

} // namespace Engine::ECS
