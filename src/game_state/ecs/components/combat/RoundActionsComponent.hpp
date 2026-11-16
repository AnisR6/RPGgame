#pragma once
#include "game_state/ecs/system/combat/Structure/CombatAction.hpp"
#include <vector>

struct RoundActionsComponent {
	std::vector<CombatAction> roundActions;
};
