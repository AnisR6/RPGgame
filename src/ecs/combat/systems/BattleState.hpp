#pragma once
#include "ecs/combat/systems/CombatAction.hpp"

#include <vector>

enum class BattlePhase { PlayerTurn, EnemyAITurn, ExecuteTurn, WonBattle, LostBattle };

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
