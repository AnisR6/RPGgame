#pragma once

#include "ecs/system/ISystem.hpp"

namespace Engine::ECS {

    // standing: the standing frame of the direction we look at. Walking: the 4 frames of that direction.
    class AnimationSystem : public ISystem {
        public:
            void Update(SystemContext& context, float deltaTime) override;
    };

} // namespace Engine::ECS
