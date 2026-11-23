#include "application/overworld/mappers/OverworldMapper.hpp"

OverworldInfoResponse
OverworldMapper::MapToOverworldInfoResponse(Engine::ECS::Entity mapEntity,
                                            Engine::ECS::Entity playerEntity) {
    return OverworldInfoResponse{.mapEntity = mapEntity, .playerEntity = playerEntity};
}
