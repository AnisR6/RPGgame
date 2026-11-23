#pragma once
#include "ecs/components/combat/DeadComponent.hpp"
#include "ecs/components/combat/TargetScopeComponent.hpp"
#include "ecs/entity/Entity.hpp"
#include "ecs/system/SystemContext.hpp"
#include "ecs/system/combat/battle_state/BattleState.hpp"

#include <vector>

// TODO: add SINGLE_DEAD_ALLY and ALL_DEAD_ALLIES as TargetScope for revive spells
namespace Engine::ECS::TargetResolver {
using TargetGroup = std::vector<Entity>;

inline std::vector<TargetGroup> resolve(const TargetScope targetScope, Entity actor, const std::vector<Entity> &allies,
                                        const std::vector<Entity> &enemies)
{
	switch (targetScope) {
	case TargetScope::SELF:
		return {{actor}};

	case TargetScope::SINGLE_ALLY: {
		std::vector<TargetGroup> result;
		result.reserve(allies.size());

		for (auto ally : allies) {
			result.push_back({ally});
		}
		return result;
	}
	case TargetScope::SINGLE_ENEMY: {
		std::vector<TargetGroup> result;
		result.reserve(enemies.size());

		for (auto enemy : enemies) {
			result.push_back({enemy});
		}
		return result;
	}
	case TargetScope::ALL_ALLIES: {
		return allies.empty() ? std::vector<TargetGroup>{} : std::vector<TargetGroup>{allies};
	}
	case TargetScope::ALL_ENEMIES: {
		return enemies.empty() ? std::vector<TargetGroup>{} : std::vector<TargetGroup>{enemies};
	}
	}

	return {};
}

inline bool retarget(const SystemContext &context, const BattleState &state, CombatAction &action)
{
	const auto &targetScope = context.GetComponent<TargetScopeComponent>(action.actionEntity).targetScope;
	const auto &players = state.turnOrderState.playerPartyEntities;
	const auto &enemies = state.turnOrderState.enemyEntities;

	const bool actorIsPlayer = std::ranges::find(players, action.actorEntity) != players.end();
	const auto &allies = actorIsPlayer ? players : enemies;
	const auto &opponents = actorIsPlayer ? enemies : players;

	switch (targetScope) {
	case TargetScope::SINGLE_ENEMY:
	case TargetScope::SINGLE_ALLY: {
		const auto &candidates = targetScope == TargetScope::SINGLE_ENEMY ? opponents : allies;

		if (!action.targetEntities.empty()) {
			// check if original target is still living
			const auto originalTarget = action.targetEntities.front();

			if (std::ranges::find(candidates, originalTarget) != candidates.end()
			    && !context.HasComponent<DeadComponent>(originalTarget)) {
				return true;
			}
		}

		for (const auto candidate : candidates) {
			if (!context.HasComponent<DeadComponent>(candidate)) {
				action.targetEntities = {candidate};
				return true;
			}
		}

		// all targets are dead
		action.targetEntities.clear();
		return false;
	}
	case TargetScope::ALL_ENEMIES:
	case TargetScope::ALL_ALLIES: {
		const auto &candidates = targetScope == TargetScope::ALL_ENEMIES ? opponents : allies;
		action.targetEntities.clear();

		for (const auto candidate : candidates) {
			if (!context.HasComponent<DeadComponent>(candidate)) {
				action.targetEntities.push_back(candidate);
			}
		}

		return !action.targetEntities.empty();
	}
	case TargetScope::SELF:
		action.targetEntities = {action.actorEntity};
		return !context.HasComponent<DeadComponent>(action.actorEntity);
	}
	return false;
}
} // namespace Engine::ECS::TargetResolver