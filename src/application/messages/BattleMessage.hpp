#pragma once
#include "application/battle/CombatantInfo.hpp"
#include "game_logic/systems/game/battle/battle_system/Structure/CombatAction.hpp"

#include <variant>

enum class CombatantType { PARTY_MEMBER, ENEMY };
// identify actors and targets within their CombatantInfo vectors
struct CombatantId {
	CombatantType type;
	std::size_t index;
};

// this event gets all spells, skills, items in inventory, etc.
// when the battle starts
struct BattleInfoRequest {};

struct BattleInfoResponse {
	std::vector<CombatantInfo> playerPartyInfo;
	std::vector<CombatantInfo> enemyPartyInfo;
};

struct CombatActionRequest {
	CombatantId actor;
	std::vector<CombatantId> targets;
	ActionType actionType = ActionType::BASE_ATTACK;
	int actionId = -1;
};

using BattleMessage = std::variant<BattleInfoRequest, BattleInfoResponse, CombatActionRequest>;