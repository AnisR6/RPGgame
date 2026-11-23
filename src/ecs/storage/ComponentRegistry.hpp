
#pragma once

#include "ecs/entity/Entity.hpp"
#include "ComponentPool.hpp"
#include "IComponentPool.hpp"

#include <cstddef>
#include <memory>
#include <type_traits>
#include <unordered_map>
#include <utility>

namespace Engine::ECS {
    class ComponentRegistry {
        public:
            // ------------------------------------------------------------
            // Construction
            // ------------------------------------------------------------

            ComponentRegistry() = default;

            ComponentRegistry(const ComponentRegistry&) = delete;
            ComponentRegistry& operator=(const ComponentRegistry&) = delete;

            ComponentRegistry(ComponentRegistry&&) = delete;
            ComponentRegistry& operator=(ComponentRegistry&&) = delete;

            ~ComponentRegistry() = default;

            // ------------------------------------------------------------
            // Validation
            // ------------------------------------------------------------

            template <typename T>
            static constexpr void ValidateComponentType() {
                static_assert(std::is_same_v<T, std::remove_cvref_t<T>>,
                              "Component types must not have cv-qualifiers or references.");
            }

            // ------------------------------------------------------------
            // Pool Management
            // ------------------------------------------------------------

            template <typename T> ComponentPool<T>& GetOrCreatePool();

            template <typename T>
            [[nodiscard]]
            ComponentPool<T>* GetPool() noexcept;

            template <typename T>
            [[nodiscard]]
            ComponentPool<T>* GetPool() const noexcept;

            // ------------------------------------------------------------
            // Component Management
            // ------------------------------------------------------------

            void RemoveComponents(Entity::Index entityIndex) noexcept;

            void Clear() noexcept;

            // ------------------------------------------------------------
            // Capacity
            // ------------------------------------------------------------

            void ReservePools(std::size_t poolCount);

            // ------------------------------------------------------------
            // Accessors
            // ------------------------------------------------------------

            [[nodiscard]]
            std::size_t GetPoolCount() const noexcept;

        private:
            // ------------------------------------------------------------
            // Component Type IDs
            // ------------------------------------------------------------

            template <typename T>
            [[nodiscard]]
            static ComponentTypeId GetComponentTypeId() noexcept;

            static ComponentTypeId nextComponentTypeId;

            // ------------------------------------------------------------
            // Storage
            // ------------------------------------------------------------

            std::unordered_map<ComponentTypeId, std::unique_ptr<IComponentPool>> pools;
    };

    // ------------------------------------------------------------
    // GetOrCreatePool
    // ------------------------------------------------------------

    template <typename T> ComponentPool<T>& ComponentRegistry::GetOrCreatePool() {
        ValidateComponentType<T>();

        const ComponentTypeId typeId = GetComponentTypeId<T>();

        auto poolIterator = pools.find(typeId);

        if (poolIterator != pools.end()) {
            return *static_cast<ComponentPool<T>*>(poolIterator->second.get());
        }

        auto newPool = std::make_unique<ComponentPool<T>>(typeId);
        auto insertionResult = pools.emplace(typeId, std::move(newPool));

        return *static_cast<ComponentPool<T>*>(insertionResult.first->second.get());
    }

    // ------------------------------------------------------------
    // GetPool
    // ------------------------------------------------------------

    template <typename T> ComponentPool<T>* ComponentRegistry::GetPool() noexcept {
        return static_cast<const ComponentRegistry*>(this)->GetPool<T>();
    }

    template <typename T> ComponentPool<T>* ComponentRegistry::GetPool() const noexcept {
        ValidateComponentType<T>();

        auto poolIterator = pools.find(GetComponentTypeId<T>());

        if (poolIterator == pools.end())
            return nullptr;

        return static_cast<ComponentPool<T>*>(poolIterator->second.get());
    }

    // ------------------------------------------------------------
    // GetComponentTypeId
    // ------------------------------------------------------------

    template <typename T> ComponentTypeId ComponentRegistry::GetComponentTypeId() noexcept {
        static const ComponentTypeId typeId = nextComponentTypeId++;
        return typeId;
    }

    // ------------------------------------------------------------
    // RemoveComponents
    // ------------------------------------------------------------

    inline void ComponentRegistry::RemoveComponents(Entity::Index entityIndex) noexcept {
        for (auto& [typeId, pool] : pools)
            pool->RemoveComponent(entityIndex);
    }

    // ------------------------------------------------------------
    // Clear
    // ------------------------------------------------------------

    inline void ComponentRegistry::Clear() noexcept {
        pools.clear();
    }

    // ------------------------------------------------------------
    // ReservePools
    // ------------------------------------------------------------

    inline void ComponentRegistry::ReservePools(std::size_t poolCount) {
        pools.reserve(poolCount);
    }

    // ------------------------------------------------------------
    // GetPoolCount
    // ------------------------------------------------------------

    inline std::size_t ComponentRegistry::GetPoolCount() const noexcept {
        return pools.size();
    }

    // ------------------------------------------------------------
    // ComponentTypeId
    // ------------------------------------------------------------

    inline ComponentTypeId ComponentRegistry::nextComponentTypeId = 0;

} // namespace Engine::ECS
