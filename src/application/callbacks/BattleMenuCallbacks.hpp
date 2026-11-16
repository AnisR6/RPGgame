#pragma once
#include "game_state/ecs/entity/Entity.hpp"

#include <functional>

class BattleMenuCallbacks {

  public:
	std::function<void(std::vector<Engine::ECS::Entity> targetEntities)> onBaseAttack;
	std::function<void(std::vector<Engine::ECS::Entity> targetEntities, int spellId)> onSpell;
	std::function<void(std::vector<Engine::ECS::Entity> targetEntities, int skillId)> onSkill;
	std::function<void(std::vector<Engine::ECS::Entity> targetEntities, int itemId)> onInventory;
	std::function<void()> onFlight;
};