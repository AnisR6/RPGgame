#pragma once

#include "game_state/ecs/system/ISystem.hpp"

namespace Engine::ECS {

    class MovementSystem : public ISystem {
        public:
            void Update(SystemContext& context, float deltaTime) override;
    };

} // namespace Engine::ECS
