#pragma once

#include "ecs/system/ISystem.hpp"

namespace Engine::ECS {

    class InputSystem : public ISystem {
        public:
            void Update(SystemContext& context, float deltaTime) override;

        private:
            bool wasInteractKeyDown = false; // E in the last frame
    };

} // namespace Engine::ECS
