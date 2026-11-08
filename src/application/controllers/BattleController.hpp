#pragma once

#include "application/battle/CombatantInfo.hpp"
#include "application/bus/BattleMessageBus.hpp"
#include "application/callbacks/BattleMenuCallbacks.hpp"

class BattleController {
  private:
	BattleMessageBus &battleMessageBus;
	std::vector<CombatantInfo> partyMembers;
	std::vector<CombatantInfo> enemies;
	BattleMenuCallbacks callbacks;
	void setCallbacksForBattleScreen();
	void setBattleInfoForBattleScreen();
	void updateBattleInfo(const BattleInfoResponse &response);

  public:
	explicit BattleController(BattleMessageBus &eventBus);
	void initBattle();
};