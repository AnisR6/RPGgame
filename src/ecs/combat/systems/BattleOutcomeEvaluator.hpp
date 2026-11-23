#pragma once

#include "ecs/combat/components/StatsComponent.hpp"
#include "ecs/core/systems/SystemContext.hpp"
#include "ecs/combat/systems/BattleState.hpp"

#include <algorithm>

enum class BattleOutcome { Ongoing, Victory, Defeat, Escaped };

namespace BattleOutcomeEvaluator {
    inline BattleOutcome Evaluate(const Engine::ECS::SystemContext& context,
                                  const BattleState& battleState) {
        const auto isAlive = [&context](const Engine::ECS::Entity entity) {
            return context.GetComponent<StatsComponent>(entity).currHP > 0;
        };

        const auto& partyMembers = battleState.turnOrderState.playerPartyEntities;
        const auto& enemies = battleState.turnOrderState.enemyEntities;

        if (!std::ranges::any_of(enemies, isAlive)) {
            return BattleOutcome::Victory;
        }

        if (!std::ranges::any_of(partyMembers, isAlive)) {
            return BattleOutcome::Defeat;
        }

        return BattleOutcome::Ongoing;
    }
}
