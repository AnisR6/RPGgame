#pragma once
#include "game_state/ecs/entity/Entity.hpp"
#include "game_state/ecs/system/combat/Structure/CombatAction.hpp"

#include <variant>

// this request gets all spells, skills, items in inventory, etc.
// when the battle starts
struct BattleInfoRequest {};

struct BattleInfoResponse {
	std::vector<Engine::ECS::Entity> playerPartyEntities;
	std::vector<Engine::ECS::Entity> enemyPartyEntities;
	std::size_t currentActorIndex;
};

struct CombatActionRequest {
	Engine::ECS::Entity actorEntity;
	std::vector<Engine::ECS::Entity> targetEntities;
	ActionType actionType = ActionType::BASE_ATTACK;
	int actionId = -1;
};

using BattleMessage = std::variant<BattleInfoRequest, BattleInfoResponse, CombatActionRequest>;