#pragma once

#include "application/overworld/messages/OverworldMessage.hpp"

class OverworldMapper {
    public:
        static OverworldInfoResponse MapToOverworldInfoResponse(Engine::ECS::Entity mapEntity,
                                                                Engine::ECS::Entity playerEntity);
};
