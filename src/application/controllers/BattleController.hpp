#pragma once

#include "application/bus/BattleMessageBus.hpp"
#include "application/callbacks/BattleMenuCallbacks.hpp"
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

  public:
	explicit BattleController(BattleMessageBus &messageBus, Engine::ECS::World &world);
	void initBattle();
};