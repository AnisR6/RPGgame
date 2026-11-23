#pragma once
#include "game_state/ecs/entity/Entity.hpp"

#include <string>
#include <variant>
#include <vector>

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
	Engine::ECS::Entity actionEntity;
};

struct ShowBattleDialogRequest {
	Engine::ECS::Entity actorEntity;
	Engine::ECS::Entity actionEntity;
	std::vector<Engine::ECS::Entity> targetEntities;
};

struct ShowBattleDialogFinished {};

struct PlayBattleAnimationRequest {
	Engine::ECS::Entity actorEntity;
	Engine::ECS::Entity actionEntity;
	std::vector<Engine::ECS::Entity> targetEntities;
};

struct PlayBattleAnimationFinished {};

struct PlayerTurnStarted {};

using BattleMessage =
    std::variant<BattleInfoRequest, BattleInfoResponse, CombatActionRequest, ShowBattleDialogRequest,
                 ShowBattleDialogFinished, PlayBattleAnimationRequest, PlayBattleAnimationFinished, PlayerTurnStarted>;