#pragma once

#include "ecs/core/entity/Entity.hpp"
#include "ecs/core/storage/IComponentPool.hpp"
#include "ecs/core/storage/SparseSet.hpp"

#include <cstddef>
#include <stdexcept>
#include <type_traits>
#include <utility>
#include <vector>

namespace Engine::ECS {
    template <typename T> class ComponentPool : public IComponentPool {
            static_assert(std::is_nothrow_move_assignable_v<T>,
                          "Component types must be nothrow move assignable.");

            static_assert(std::is_nothrow_destructible_v<T>,
                          "Component types must be nothrow destructible.");

        public:
            explicit ComponentPool(ComponentTypeId componentPoolTypeId) noexcept
                : componentPoolTypeId(componentPoolTypeId) {}

            ComponentPool(const ComponentPool&) = delete;
            ComponentPool& operator=(const ComponentPool&) = delete;

            ComponentPool(ComponentPool&&) = delete;
            ComponentPool& operator=(ComponentPool&&) = delete;

            template <typename... Args>
            T& EmplaceComponent(Entity::Index entityIndex, Args&&... args);

            bool RemoveComponent(Entity::Index entityIndex) noexcept override;

            T& GetComponent(Entity::Index entityIndex);

            [[nodiscard]]
            bool ContainsComponent(Entity::Index entityIndex) const noexcept;

            void Reserve(std::size_t capacity);

            [[nodiscard]]
            std::size_t GetSize() const noexcept override;

            [[nodiscard]]
            bool IsEmpty() const noexcept;

            [[nodiscard]]
            Entity::Index GetEntityAt(std::size_t packedIndex) const noexcept;

            [[nodiscard]]
            ComponentTypeId GetComponentPoolTypeId() const noexcept override;

        private:
            SparseSet entitySet;
            std::vector<T> components;
            ComponentTypeId componentPoolTypeId;
    };

    template <typename T>
    template <typename... Args>
    T& ComponentPool<T>::EmplaceComponent(Entity::Index entityIndex, Args&&... args) {
        if (!entitySet.Insert(entityIndex))
            throw std::logic_error("Failed to insert component.");

        try {
            components.emplace_back(std::forward<Args>(args)...);
        } catch (...) {
            entitySet.Remove(entityIndex);
            throw;
        }

        return components.back();
    }

    template <typename T>
    bool ComponentPool<T>::RemoveComponent(Entity::Index entityIndex) noexcept {
        const Entity::Index packedIndex = entitySet.GetPackedIndex(entityIndex);

        if (packedIndex == SparseSet::InvalidIndex)
            return false;

        const std::size_t index = packedIndex;
        const std::size_t lastIndex = components.size() - 1;

        if (index != lastIndex)
            components[index] = std::move(components[lastIndex]);

        components.pop_back();
        entitySet.Remove(entityIndex);

        return true;
    }

    template <typename T> T& ComponentPool<T>::GetComponent(Entity::Index entityIndex) {
        const Entity::Index packedIndex = entitySet.GetPackedIndex(entityIndex);

        if (packedIndex == SparseSet::InvalidIndex)
            throw std::out_of_range("Entity does not have this component.");

        return components[packedIndex];
    }

    template <typename T>
    bool ComponentPool<T>::ContainsComponent(Entity::Index entityIndex) const noexcept {
        return entitySet.Contains(entityIndex);
    }

    template <typename T> void ComponentPool<T>::Reserve(std::size_t capacity) {
        entitySet.Reserve(capacity);
        components.reserve(capacity);
    }

    template <typename T> std::size_t ComponentPool<T>::GetSize() const noexcept {
        return components.size();
    }

    template <typename T> bool ComponentPool<T>::IsEmpty() const noexcept {
        return components.empty();
    }

    template <typename T>
    Entity::Index ComponentPool<T>::GetEntityAt(std::size_t packedIndex) const noexcept {
        return entitySet.GetPackedEntity(packedIndex);
    }

    template <typename T>
    ComponentTypeId ComponentPool<T>::GetComponentPoolTypeId() const noexcept {
        return componentPoolTypeId;
    }
}
