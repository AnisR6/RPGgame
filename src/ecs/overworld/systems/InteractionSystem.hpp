#pragma once

#include "ecs/core/systems/ISystem.hpp"
#include "ecs/overworld/systems/OverworldState.hpp"

class InteractionSystem : public Engine::ECS::ISystem {
    public:
        explicit InteractionSystem(OverworldState& overworldState);

        void Update(Engine::ECS::SystemContext& context, float deltaTime) override;

    private:
        OverworldState& overworldState;
};
