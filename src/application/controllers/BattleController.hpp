#pragma once

#include "application/battle/CombatantInfo.hpp"
#include "application/bus/BattleMessageBus.hpp"
#include "application/callbacks/BattleMenuCallbacks.hpp"

class BattleController {
  private:
	BattleMessageBus &battleMessageBus;
	std::vector<CombatantInfo> partyMembers;
	std::vector<CombatantInfo> enemies;
	std::size_t currentPartyMemberIndex = 0;

	BattleMenuCallbacks callbacks;
	void setCallbacksForBattleScreen();
	void setBattleInfoForBattleScreen() const;
	void updateBattleInfo(const BattleInfoResponse &response);

  public:
	explicit BattleController(BattleMessageBus &messageBus);
	void initBattle();
};