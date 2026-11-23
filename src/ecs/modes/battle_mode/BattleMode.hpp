#pragma once
#include "application/bus/BattleMessageBus.hpp"
#include "application/mappers/BattleMapper.hpp"
#include "ecs/system/combat/battle_state/BattleState.hpp"
#include "ecs/system/combat/combat_action/combat_action_executor/CombatActionExecutor.hpp"
#include "ecs/world/World.hpp"

namespace Engine::ECS {
class BattleMode {
  private:
	BattleMessageBus &battleModeMessageBus;
	World &world;
	SystemManager systemManager;
	BattleState battleState;
	CombatActionExecutor executor;

	void subscribeToEvents();
	void registerSystems();

	void handleBattleInfoRequest() const;
	void handleCombatActionRequest(const CombatActionRequest &combatActionRequest);

	// To be removed; helper for hardcoding party members and enemies
	void createParties(std::vector<Entity> &partyMembers, std::vector<Entity> &enemies) const;

  public:
	BattleMode(BattleMessageBus &messageBus, World &world, BattleState battleState, CombatActionExecutor executor);

	void startBattle();
	void Update(float deltaTime);
	void endBattle() const;
};
} // namespace Engine::ECS