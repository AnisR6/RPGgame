#pragma once

#include "application/bus/BattleMessageBus.hpp"
#include "application/callbacks/BattleMenuCallbacks.hpp"
#include "application/menu_entries/BattleActionMenuEntry.hpp"
#include "application/menu_entries/CombatantMenuEntry.hpp"
#include "game_state/ecs/world/World.hpp"

class BattleController {
  private:
	BattleMessageBus &battleMessageBus;
	Engine::ECS::World &world;
	std::vector<Engine::ECS::Entity> partyMembers;
	std::vector<Engine::ECS::Entity> enemies;
	std::size_t currentActorIndex{};
	BattleMenuCallbacks callbacks;

	void setCallbacksForBattleScreen();
	void setBattleInfoForBattleScreen() const;
	void setBattleActorMenuDataForBattleScreen() const;
	void updateBattleInfo(const BattleInfoResponse &response);

	[[nodiscard]] BattleActionMenuEntry createActionEntry(Engine::ECS::Entity actionEntity) const;
	[[nodiscard]] CombatantMenuEntry createCombatantMenuEntry(Engine::ECS::Entity combatantEntity) const;

	void showBattleDialog(const ShowBattleDialogRequest &request) const;
	void playBattleAnimation(const PlayBattleAnimationRequest &request) const;

  public:
	explicit BattleController(BattleMessageBus &messageBus, Engine::ECS::World &world);
	void requestBattleInfo() const;
};