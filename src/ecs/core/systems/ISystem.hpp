#pragma once

#include "ecs/core/systems/SystemContext.hpp"

namespace Engine::ECS {
    class ISystem {
        public:
            virtual ~ISystem() = default;

            virtual void Update(SystemContext& context, float deltaTime) = 0;
    };
}
