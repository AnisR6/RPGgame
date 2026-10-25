#pragma once

#include "BattleState.hpp"
#include "Combat/Structure/CombatAction.hpp"

class Battle {
	private:
		BattleState battleState;
		void getPartyMemberActions(std::vector<CombatAction>& roundActions);
		void getEnemyActions(std::vector<CombatAction>& roundActions);

	public:
		void runRound();
};