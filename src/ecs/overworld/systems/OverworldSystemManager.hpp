#pragma once

#include "ecs/core/systems/SystemManager.hpp"
#include "ecs/overworld/systems/OverworldState.hpp"

class OverworldSystemManager : public Engine::ECS::SystemManager {
    public:
        explicit OverworldSystemManager(OverworldState& overworldState);
};
