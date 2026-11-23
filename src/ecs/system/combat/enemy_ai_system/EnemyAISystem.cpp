#include "EnemyAISystem.hpp"

#include "../target_resolver/TargetResolver.hpp"
#include "ecs/components/combat/TargetScopeComponent.hpp"

#include <algorithm>

namespace Engine::ECS {

std::vector<CombatAction> EnemyAISystem::possibleActions(const SystemContext &context, const Entity actor,
                                                         const std::vector<Entity> &knownActions) const
{
	std::vector<CombatAction> posActions;
	for (const auto action : knownActions) {
		const auto targetScope = context.GetComponent<TargetScopeComponent>(action).targetScope;

		// the allies and enemies are seen from the actor's perspective
		const auto targetGroups = TargetResolver::resolve(targetScope, actor, battleState.turnOrderState.enemyEntities,
		                                                  battleState.turnOrderState.playerPartyEntities);

		for (const auto targets : targetGroups) {
			posActions.push_back({.actorEntity = actor, .targetEntities = targets, .actionEntity = action});
		}
	}

	return posActions;
}

float EnemyAISystem::valuateAction(SystemContext &context, const CombatAction &action, const EnemyAIProfile &profile,
                                   const StatsComponent &actorStats)
{
	// TODO
	return 0.f;
}

std::vector<ScoredAction> EnemyAISystem::valuateActions(SystemContext &context, Entity actor,
                                                        const EnemyAIProfile &profile,
                                                        const std::vector<Entity> &knownActions,
                                                        const StatsComponent &actorStats)
{
	// TODO
	return {};
}

CombatAction EnemyAISystem::chooseHighestValuatedAction(SystemContext &context, Entity actor,
                                                        const EnemyAIProfile &profile,
                                                        const std::vector<Entity> &knownActions,
                                                        const StatsComponent &actorStats)
{
	auto actions = possibleActions(context, actor, knownActions);
	if (actions.empty()) {
		throw std::runtime_error("EnemyAISystem: Enemy has no available actions");
	}

	// TODO: replace with real chosen action later
	return actions.front();
}
} // namespace Engine::ECS