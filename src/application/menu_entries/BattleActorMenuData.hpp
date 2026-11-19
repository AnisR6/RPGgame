#pragma once
#include "BattleActionMenuEntry.hpp"

#include <vector>

struct BattleActorMenuData {
	BattleActionMenuEntry baseAttack;
	BattleActionMenuEntry flight;
	std::vector<BattleActionMenuEntry> spells;
	std::vector<BattleActionMenuEntry> skills;
};
