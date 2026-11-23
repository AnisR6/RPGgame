#pragma once

#include "ecs/core/systems/ISystem.hpp"

class MovementSystem : public Engine::ECS::ISystem {
    public:
        void Update(Engine::ECS::SystemContext& context, float deltaTime) override;
};
