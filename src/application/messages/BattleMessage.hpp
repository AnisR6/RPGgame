#pragma once
#include "application/battle/CombatantInfo.hpp"
#include "game_logic/systems/game/battle/battle_system/Structure/CombatAction.hpp"

#include <variant>

// this request gets all spells, skills, items in inventory, etc.
// when the battle starts
struct BattleInfoRequest {};

struct BattleInfoResponse {
	std::vector<CombatantInfo> playerPartyInfo;
	std::vector<CombatantInfo> enemyPartyInfo;
	std::size_t currentPartyMemberIndex;
};

struct CombatActionRequest {
	int actorId;
	std::vector<int> targetIds;
	ActionType actionType = ActionType::BASE_ATTACK;
	int actionId = -1;
};

using BattleMessage = std::variant<BattleInfoRequest, BattleInfoResponse, CombatActionRequest>;