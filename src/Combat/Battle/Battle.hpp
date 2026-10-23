#pragma once

#include "BattleState.hpp"

class Battle {
	private:
		BattleState battleState;

	public:
		void runRound();
};