#pragma once

#include "ecs/core/systems/ISystem.hpp"

class AnimationSystem : public Engine::ECS::ISystem {
    public:
        void Update(Engine::ECS::SystemContext& context, float deltaTime) override;
};
