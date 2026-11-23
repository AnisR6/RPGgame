#pragma once
#include "ecs/components/combat/StatsComponent.hpp"
#include "ecs/system/SystemContext.hpp"
#include "ecs/system/combat/battle_state/BattleState.hpp"

#include <algorithm>

// TODO: implement Escaped outcome for Flee
enum class BattleOutcome { Ongoing, Victory, Defeat, Escaped };

class BattleOutcomeEvaluator {

  public:
	static BattleOutcome evaluate(const Engine::ECS::SystemContext &context, const BattleState &battleState)
	{
		const auto isAlive = [&context](const Engine::ECS::Entity entity) {
			return context.GetComponent<StatsComponent>(entity).currHP > 0;
		};

		const auto &partyMembers = battleState.turnOrderState.playerPartyEntities;
		const auto &enemies = battleState.turnOrderState.enemyEntities;
		const bool partyMembersAlive = std::ranges::any_of(partyMembers, isAlive);
		const bool enemiesAlive = std::ranges::any_of(enemies, isAlive);

		if (!enemiesAlive) {
			return BattleOutcome::Victory;
		}
		if (!partyMembersAlive) {
			return BattleOutcome::Defeat;
		}
		return BattleOutcome::Ongoing;
	}
};