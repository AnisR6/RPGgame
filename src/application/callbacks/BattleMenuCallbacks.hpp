#pragma once
#include "game_state/ecs/entity/Entity.hpp"

#include <functional>

class BattleMenuCallbacks {

  public:
	std::function<void(std::vector<Engine::ECS::Entity> targetEntities, Engine::ECS::Entity actionEntity)>
	    onActionSelected;
};