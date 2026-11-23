#pragma once

#include "ecs/core/entity/Entity.hpp"
#include "ecs/core/storage/ComponentPool.hpp"
#include "ecs/core/storage/IComponentPool.hpp"

#include <cstddef>
#include <memory>
#include <type_traits>
#include <unordered_map>
#include <utility>

namespace Engine::ECS {
    class ComponentRegistry {
        public:
            ComponentRegistry() = default;

            ComponentRegistry(const ComponentRegistry&) = delete;
            ComponentRegistry& operator=(const ComponentRegistry&) = delete;

            ComponentRegistry(ComponentRegistry&&) = delete;
            ComponentRegistry& operator=(ComponentRegistry&&) = delete;

            ~ComponentRegistry() = default;

            template <typename T> static constexpr void ValidateComponentType() {
                static_assert(std::is_same_v<T, std::remove_cvref_t<T>>,
                              "Component types must not have cv-qualifiers or references.");
            }

            template <typename T> ComponentPool<T>& GetOrCreatePool();

            template <typename T>
            [[nodiscard]]
            ComponentPool<T>* GetPool() noexcept;

            template <typename T>
            [[nodiscard]]
            ComponentPool<T>* GetPool() const noexcept;

            void RemoveComponents(Entity::Index entityIndex) noexcept;

            void Clear() noexcept;

            void ReservePools(std::size_t poolCount);

            [[nodiscard]]
            std::size_t GetPoolCount() const noexcept;

        private:
            template <typename T>
            [[nodiscard]]
            static ComponentTypeId GetComponentTypeId() noexcept;

            static ComponentTypeId nextComponentTypeId;

            std::unordered_map<ComponentTypeId, std::unique_ptr<IComponentPool>> pools;
    };

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

    template <typename T> ComponentTypeId ComponentRegistry::GetComponentTypeId() noexcept {
        static const ComponentTypeId typeId = nextComponentTypeId++;
        return typeId;
    }

    inline void ComponentRegistry::RemoveComponents(Entity::Index entityIndex) noexcept {
        for (auto& [typeId, pool] : pools)
            pool->RemoveComponent(entityIndex);
    }

    inline void ComponentRegistry::Clear() noexcept {
        pools.clear();
    }

    inline void ComponentRegistry::ReservePools(std::size_t poolCount) {
        pools.reserve(poolCount);
    }

    inline std::size_t ComponentRegistry::GetPoolCount() const noexcept {
        return pools.size();
    }

    inline ComponentTypeId ComponentRegistry::nextComponentTypeId = 0;
}
