#pragma once

#include "application/overworld/bus/OverworldMessageBus.hpp"
#include "ecs/core/world/World.hpp"

class OverworldScreen;

class OverworldController {
    public:
        OverworldController(OverworldMessageBus& messageBus, OverworldScreen& screen,
                            Engine::ECS::World& world);

        void RequestOverworldInfo() const;

    private:
        void UpdateOverworldInfo(const OverworldInfoResponse& response);

        OverworldMessageBus& overworldMessageBus;
        OverworldScreen& screen;
        Engine::ECS::World& world;
};
