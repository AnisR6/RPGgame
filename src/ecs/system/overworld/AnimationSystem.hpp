#pragma once

#include "ecs/system/ISystem.hpp"

namespace Engine::ECS {

    // -----------------------------------------------------------------------------
    // Animation System
    // -----------------------------------------------------------------------------
    class AnimationSystem : public ISystem {
        public:
            // -------------------------------------------------------------------------
            // System Update
            // -------------------------------------------------------------------------
            void Update(SystemContext& context, float deltaTime) override;
    };

} // namespace Engine::ECS