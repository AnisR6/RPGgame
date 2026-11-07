#pragma once
#include "application/battle/CombatantInfo.hpp"
#include "game_logic/systems/game/battle/battle_system/Structure/CombatAction.hpp"
#include "game_state/ecs/capabilities/Combatable.hpp"

#include <variant>

// this event gets all spells, skills, items in inventory, etc.
// when the battle starts
struct BattleInfoRequest {};

struct BattleInfoResponse {
	std::vector<CombatantInfo> playerPartyInfo;
	std::vector<CombatantInfo> enemyPartyInfo;
};

// this event contains info for the chosen action and target after a party member took its turn
struct CombatActionRequest {
	Combatable *actor = nullptr;
	std::vector<Combatable *> targets;

	ActionType actionType = ActionType::BASE_ATTACK;

	const Spell *spell = nullptr;
	const CombatSkill *skill = nullptr;
};

using BattleMessage = std::variant<BattleInfoRequest, BattleInfoResponse, CombatActionRequest>;