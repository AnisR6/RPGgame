#pragma once
#include "application/modes/battle_mode/BattleMode.hpp"
#include "ecs/components/combat/DeadComponent.hpp"
#include "ecs/components/combat/EnemyAIComponent.hpp"
#include "ecs/components/combat/KnownActionsComponent.hpp"
#include "ecs/components/combat/StatsComponent.hpp"
#include "ecs/components/combat/TargetScopeComponent.hpp"
#include "ecs/system/ISystem.hpp"
#include "ecs/system/combat/combat_action/CombatAction.hpp"

struct ScoredAction {
	CombatAction action;
	float value = 0.f;

	bool operator<(const ScoredAction &other) const { return this->value < other.value; }
};

namespace Engine::ECS {
class EnemyAISystem : public ISystem {
  private:
	BattleState &battleState;

	[[nodiscard]] std::vector<CombatAction> possibleActions(const SystemContext &context, Entity actor,
	                                                        const std::vector<Entity> &knownActions) const;

	float valuateAction(SystemContext &context, const CombatAction &action, const EnemyAIProfile &profile,
	                    const StatsComponent &actorStats);

	std::vector<ScoredAction> valuateActions(SystemContext &context, Entity actor, const EnemyAIProfile &profile,
	                                         const std::vector<Entity> &knownActions, const StatsComponent &actorStats);

	CombatAction chooseHighestValuatedAction(SystemContext &context, Entity actor, const EnemyAIProfile &profile,
	                                         const std::vector<Entity> &knownActions, const StatsComponent &actorStats);

  public:
	explicit EnemyAISystem(BattleState &battleState) : battleState(battleState) {}

	void Update(SystemContext &context, float deltaTime) override
	{
		if (battleState.battlePhaseState.currentPhase != BattlePhase::EnemyAITurn) {
			return;
		}

		// chose action for each enemy, push action onto roundActions
		const auto query = context.CreateQuery<Including<EnemyAIComponent, KnownActionsComponent, StatsComponent>,
		                                       Excluding<DeadComponent>>();
		query.ForEach([this, &context](const Entity entity, const EnemyAIComponent &ai,
		                               const KnownActionsComponent &knownActions, const StatsComponent &stats) {
			const auto bestAction =
			    chooseHighestValuatedAction(context, entity, ai.profile, knownActions.knownActionEntities, stats);

			battleState.roundActionsState.roundActions.push_back(bestAction);
		});

		battleState.battlePhaseState.currentPhase = BattlePhase::ExecuteTurn;
	}
};
} // namespace Engine::ECS