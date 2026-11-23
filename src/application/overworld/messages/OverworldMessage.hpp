#pragma once

#include "ecs/core/entity/Entity.hpp"

#include <variant>

struct OverworldInfoRequest {};

struct OverworldInfoResponse {
        Engine::ECS::Entity mapEntity;
        Engine::ECS::Entity playerEntity;
};

using OverworldMessage = std::variant<OverworldInfoRequest, OverworldInfoResponse>;
