#pragma once
#include "ecs/combat/components/DeadComponent.hpp"
#include "ecs/combat/components/TargetScopeComponent.hpp"
#include "ecs/core/entity/Entity.hpp"
#include "ecs/core/systems/SystemContext.hpp"
#include "ecs/combat/systems/BattleState.hpp"

#include <vector>

namespace TargetResolver {
    using TargetGroup = std::vector<Engine::ECS::Entity>;

    inline std::vector<TargetGroup> Resolve(const TargetScope targetScope,
                                            Engine::ECS::Entity actor,
                                            const std::vector<Engine::ECS::Entity>& allies,
                                            const std::vector<Engine::ECS::Entity>& enemies) {
        switch (targetScope) {
        case TargetScope::Self:
            return {{actor}};

        case TargetScope::SingleAlly: {
            std::vector<TargetGroup> result;
            result.reserve(allies.size());

            for (auto ally : allies) {
                result.push_back({ally});
            }
            return result;
        }
        case TargetScope::SingleEnemy: {
            std::vector<TargetGroup> result;
            result.reserve(enemies.size());

            for (auto enemy : enemies) {
                result.push_back({enemy});
            }
            return result;
        }
        case TargetScope::AllAllies: {
            return allies.empty() ? std::vector<TargetGroup>{} : std::vector<TargetGroup>{allies};
        }
        case TargetScope::AllEnemies: {
            return enemies.empty() ? std::vector<TargetGroup>{} : std::vector<TargetGroup>{enemies};
        }
        }

        return {};
    }

    inline bool Retarget(const Engine::ECS::SystemContext& context, const BattleState& state,
                         CombatAction& action) {
        const auto& targetScope =
            context.GetComponent<TargetScopeComponent>(action.actionEntity).targetScope;
        const auto& players = state.turnOrderState.playerPartyEntities;
        const auto& enemies = state.turnOrderState.enemyEntities;

        const bool actorIsPlayer = std::ranges::find(players, action.actorEntity) != players.end();
        const auto& allies = actorIsPlayer ? players : enemies;
        const auto& opponents = actorIsPlayer ? enemies : players;

        switch (targetScope) {
        case TargetScope::SingleEnemy:
        case TargetScope::SingleAlly: {
            const auto& candidates = targetScope == TargetScope::SingleEnemy ? opponents : allies;

            if (!action.targetEntities.empty()) {
                const auto originalTarget = action.targetEntities.front();

                if (std::ranges::find(candidates, originalTarget) != candidates.end() &&
                    !context.HasComponent<DeadComponent>(originalTarget)) {
                    return true;
                }
            }

            for (const auto candidate : candidates) {
                if (!context.HasComponent<DeadComponent>(candidate)) {
                    action.targetEntities = {candidate};
                    return true;
                }
            }

            action.targetEntities.clear();
            return false;
        }
        case TargetScope::AllEnemies:
        case TargetScope::AllAllies: {
            const auto& candidates = targetScope == TargetScope::AllEnemies ? opponents : allies;
            action.targetEntities.clear();

            for (const auto candidate : candidates) {
                if (!context.HasComponent<DeadComponent>(candidate)) {
                    action.targetEntities.push_back(candidate);
                }
            }

            return !action.targetEntities.empty();
        }
        case TargetScope::Self:
            action.targetEntities = {action.actorEntity};
            return !context.HasComponent<DeadComponent>(action.actorEntity);
        }
        return false;
    }
}
