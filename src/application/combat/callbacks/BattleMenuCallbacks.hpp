#pragma once
#include "ecs/core/entity/Entity.hpp"

#include <functional>

class BattleMenuCallbacks {
    public:
        std::function<void(std::vector<Engine::ECS::Entity> targetEntities,
                           Engine::ECS::Entity actionEntity)>
            onActionSelected;
};
