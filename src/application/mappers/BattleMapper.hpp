#pragma once
#include "application/messages/BattleMessage.hpp"
#include "ecs/system/combat/combat_action/CombatAction.hpp"
#include "ecs/world/World.hpp"

namespace Engine::ECS {
}
class BattleMapper {
  private:
	Engine::ECS::World &world;

  public:
	static CombatAction mapToCombatAction(const CombatActionRequest &request);

	explicit BattleMapper(Engine::ECS::World &world) : world(world) {}
};