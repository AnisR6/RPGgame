#include "ecs/combat/systems/EnemyAISystem.hpp"

#include "ecs/combat/systems/TargetResolver.hpp"
#include "ecs/combat/components/TargetScopeComponent.hpp"

#include <algorithm>

std::vector<CombatAction>
EnemyAISystem::PossibleActions(const Engine::ECS::SystemContext& context,
                               const Engine::ECS::Entity actor,
                               const std::vector<Engine::ECS::Entity>& knownActions) const {
    std::vector<CombatAction> posActions;
    for (const auto action : knownActions) {
        const auto targetScope = context.GetComponent<TargetScopeComponent>(action).targetScope;

        const auto targetGroups =
            TargetResolver::Resolve(targetScope, actor, battleState.turnOrderState.enemyEntities,
                                    battleState.turnOrderState.playerPartyEntities);

        for (const auto targets : targetGroups) {
            posActions.push_back(
                {.actorEntity = actor, .targetEntities = targets, .actionEntity = action});
        }
    }

    return posActions;
}

float EnemyAISystem::ValuateAction(Engine::ECS::SystemContext& context, const CombatAction& action,
                                   const EnemyAIProfile& profile,
                                   const StatsComponent& actorStats) {
    return 0.f;
}

std::vector<ScoredAction> EnemyAISystem::ValuateActions(
    Engine::ECS::SystemContext& context, Engine::ECS::Entity actor, const EnemyAIProfile& profile,
    const std::vector<Engine::ECS::Entity>& knownActions, const StatsComponent& actorStats) {
    return {};
}

CombatAction EnemyAISystem::ChooseHighestValuatedAction(
    Engine::ECS::SystemContext& context, Engine::ECS::Entity actor, const EnemyAIProfile& profile,
    const std::vector<Engine::ECS::Entity>& knownActions, const StatsComponent& actorStats) {
    auto actions = PossibleActions(context, actor, knownActions);
    if (actions.empty()) {
        throw std::runtime_error("EnemyAISystem: Enemy has no available actions");
    }

    return actions.front();
}
