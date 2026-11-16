#pragma once
#include "application/battle/CombatantInfo.hpp"
#include "application/messages/BattleMessage.hpp"

class BattleMapper {
  private:
	Engine::ECS::World &world;

  public:
	static CombatAction mapToCombatAction(const CombatActionRequest &request);
	CombatantInfo mapToCombatantInfo(Engine::ECS::Entity entity) const;
	std::vector<CombatantInfo> mapToCombatantInfos(const std::vector<Engine::ECS::Entity> &combatantEntities) const;

	explicit BattleMapper(Engine::ECS::World &world) : world(world) {}
};