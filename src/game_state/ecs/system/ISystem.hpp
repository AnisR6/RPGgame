
#pragma once

#include "SystemContext.hpp"

namespace Engine::ECS {

    // ------------------------------------------------------------
    // ISystem
    // ------------------------------------------------------------

    class ISystem {
        public:
            // ------------------------------------------------------------
            // Construction
            // ------------------------------------------------------------

            virtual ~ISystem() = default;

            // ------------------------------------------------------------
            // System Update
            // ------------------------------------------------------------

            virtual void Update(SystemContext& context, float deltaTime) = 0;
    };

} // namespace Engine::ECS
