
#pragma once

#include "ISystem.hpp"
#include "SystemContext.hpp"

#include <cstddef>
#include <memory>
#include <stdexcept>
#include <type_traits>
#include <unordered_set>
#include <utility>
#include <vector>

namespace Engine::ECS {

    // ------------------------------------------------------------
    // SystemManager
    // ------------------------------------------------------------

    class SystemManager {
        public:
            // ------------------------------------------------------------
            // Construction
            // ------------------------------------------------------------

            SystemManager() = default;

            SystemManager(const SystemManager&) = delete;
            SystemManager& operator=(const SystemManager&) = delete;

            SystemManager(SystemManager&&) = delete;
            SystemManager& operator=(SystemManager&&) = delete;

            ~SystemManager() = default;

            // ------------------------------------------------------------
            // System Registration
            // ------------------------------------------------------------

            template <typename T, typename... Args> T& RegisterSystem(Args&&... args);

            // ------------------------------------------------------------
            // System Updates
            // ------------------------------------------------------------

            void Update(SystemContext& context, float deltaTime);

            // ------------------------------------------------------------
            // Accessors
            // ------------------------------------------------------------

            [[nodiscard]]
            std::size_t GetSystemCount() const noexcept;

            void Clear();

        private:
            // ------------------------------------------------------------
            // System Type IDs
            // ------------------------------------------------------------

            using SystemTypeId = std::size_t;

            template <typename T>
            [[nodiscard]]
            static SystemTypeId GetSystemTypeId() noexcept {
                static const SystemTypeId systemTypeId = nextSystemTypeId++;
                return systemTypeId;
            }

            inline static SystemTypeId nextSystemTypeId = 0;

            // ------------------------------------------------------------
            // System Registration Helpers
            // ------------------------------------------------------------

            void ValidateSystemType(SystemTypeId systemTypeId);

            template <typename T, typename... Args>
            T& CreateSystem(SystemTypeId systemTypeId, Args&&... args);

            // ------------------------------------------------------------
            // Storage
            // ------------------------------------------------------------

            std::unordered_set<SystemTypeId> registeredSystemTypes;
            std::vector<std::unique_ptr<ISystem>> systems;

            bool isUpdating = false;
    };

    // ------------------------------------------------------------
    // RegisterSystem
    // ------------------------------------------------------------

    template <typename T, typename... Args> T& SystemManager::RegisterSystem(Args&&... args) {
        static_assert(std::is_base_of_v<ISystem, T>,
                      "Registered systems must derive from ISystem.");

        const SystemTypeId systemTypeId = GetSystemTypeId<T>();

        ValidateSystemType(systemTypeId);

        return CreateSystem<T>(systemTypeId, std::forward<Args>(args)...);
    }

    // ------------------------------------------------------------
    // ValidateSystemType
    // ------------------------------------------------------------

    inline void SystemManager::ValidateSystemType(SystemTypeId systemTypeId) {
        if (isUpdating)
            throw std::logic_error("SystemManager: Cannot register systems during an update.");

        if (!registeredSystemTypes.insert(systemTypeId).second)
            throw std::logic_error("SystemManager: System type is already registered.");
    }

    // ------------------------------------------------------------
    // CreateSystem
    // ------------------------------------------------------------

    template <typename T, typename... Args>
    T& SystemManager::CreateSystem(SystemTypeId systemTypeId, Args&&... args) {
        try {
            auto newSystem = std::make_unique<T>(std::forward<Args>(args)...);

            T& registeredSystem = *newSystem;

            systems.push_back(std::move(newSystem));

            return registeredSystem;
        } catch (...) {
            registeredSystemTypes.erase(systemTypeId);
            throw;
        }
    }

    // ------------------------------------------------------------
    // Update
    // ------------------------------------------------------------

    inline void SystemManager::Update(SystemContext& context, float deltaTime) {
        if (isUpdating)
            throw std::logic_error("SystemManager: Recursive updates are not allowed.");

        isUpdating = true;

        try {
            for (const auto& system : systems)
                system->Update(context, deltaTime);
        } catch (...) {
            isUpdating = false;
            throw;
        }

        isUpdating = false;
    }

    // ------------------------------------------------------------
    // GetSystemCount
    // ------------------------------------------------------------

    inline std::size_t SystemManager::GetSystemCount() const noexcept {
        return systems.size();
    }

    // ------------------------------------------------------------
    // Clear
    // ------------------------------------------------------------

    inline void SystemManager::Clear() {
        if (isUpdating)
            throw std::logic_error("SystemManager: Cannot clear systems during an update.");

        systems.clear();
        registeredSystemTypes.clear();
    }

} // namespace Engine::ECS
