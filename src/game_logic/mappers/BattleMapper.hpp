#pragma once
#include "application/messages/BattleMessage.hpp"
#include "game_state/ecs/system/combat/combat_action/CombatAction.hpp"
#include "game_state/ecs/world/World.hpp"

namespace Engine::ECS {
}
class BattleMapper {
  private:
	Engine::ECS::World &world;

  public:
	static CombatAction mapToCombatAction(const CombatActionRequest &request);

	explicit BattleMapper(Engine::ECS::World &world) : world(world) {}
};