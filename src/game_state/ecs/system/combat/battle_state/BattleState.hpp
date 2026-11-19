#pragma once
#include "game_state/ecs/system/combat/combat_action/CombatAction.hpp"

#include <vector>

enum class BattlePhase { PlayerTurn, EnemyAITurn, ExecuteTurn, WinBattle, LoseBattle };

struct BattlePhaseState {
	BattlePhase currentPhase = BattlePhase::PlayerTurn;
};

struct RoundActionsState {
	std::vector<CombatAction> roundActions;
};

struct TurnOrderState {
	std::vector<Engine::ECS::Entity> playerPartyEntities;
	std::vector<Engine::ECS::Entity> enemyEntities;
	size_t currentActorIndex = 0;
};

struct BattleState {
	BattlePhaseState battlePhaseState;
	TurnOrderState turnOrderState;
	RoundActionsState roundActionsState;
};
