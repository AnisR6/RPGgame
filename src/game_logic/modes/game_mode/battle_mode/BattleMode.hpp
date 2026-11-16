#pragma once
#include "application/bus/BattleMessageBus.hpp"
#include "game_logic/mappers/BattleMapper.hpp"
#include "game_state/ecs/world/World.hpp"

class BattleMode {
  private:
	BattleMessageBus &battleModeMessageBus;
	Engine::ECS::World &world;
	Engine::ECS::Entity activeBattleStateEntity;

	void handleBattleInitRequest() const;
	void handleCombatActionRequest(const CombatActionRequest &combatActionRequest);

	// To be removed; helper for hardcoding party members and enemies
	void createParties(std::vector<Engine::ECS::Entity> &partyMembers, std::vector<Engine::ECS::Entity> &enemies) const;

	void startBattle();
	void endBattle() const;

  public:
	explicit BattleMode(BattleMessageBus &messageBus, Engine::ECS::World &world);
};