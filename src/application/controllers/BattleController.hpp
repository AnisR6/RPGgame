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
	void updateBattleInfo(const BattleInfoResponse &response);

	BattleActionMenuEntry createActionEntry(Engine::ECS::Entity actionEntity) const;
	CombatantMenuEntry createTargetEntry(Engine::ECS::Entity targetEntity);

	void showBattleDialog(const ShowBattleDialogRequest &request);
	void playBattleAnimation(const PlayBattleAnimationRequest &request);

  public:
	explicit BattleController(BattleMessageBus &messageBus, Engine::ECS::World &world);
	void initBattle() const;
};