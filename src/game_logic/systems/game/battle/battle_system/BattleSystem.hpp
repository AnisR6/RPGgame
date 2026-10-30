#pragma once

#include "BattleState.hpp"
#include "Structure/CombatAction.hpp"

class BattleSystem {
  private:
	BattleState battleState;
	void getPartyMemberActions(std::vector<CombatAction> &roundActions);
	void getEnemyActions(std::vector<CombatAction> &roundActions);

  public:
	void executeRound();
};