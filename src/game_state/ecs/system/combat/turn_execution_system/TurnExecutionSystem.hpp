#pragma once
#include "../combat_action/combat_action_executor/CombatActionExecutor.hpp"
#include "game_state/ecs/system/ISystem.hpp"

#include <algorithm>

namespace Engine::ECS {
class TurnExecutionSystem : public ISystem {
  private:
	BattleState &battleState;
	CombatActionExecutor executor;

  public:
	TurnExecutionSystem(BattleState &battleState, const CombatActionExecutor executor)
	    : battleState(battleState), executor(executor)
	{
	}

	void Update(SystemContext &context, float deltaTime) override
	{
		if (battleState.battlePhaseState.currentPhase != BattlePhase::ExecuteTurn) {
			return;
		}

		// sort actions by speed
		std::ranges::sort(battleState.roundActionsState.roundActions,
		                  [&context](const CombatAction &a, const CombatAction &b) {
			                  const auto &statsA = context.GetComponent<StatsComponent>(a.actorEntity);
			                  const auto &statsB = context.GetComponent<StatsComponent>(b.actorEntity);
			                  return statsA.speed > statsB.speed;
		                  });

		for (const auto &action : battleState.roundActionsState.roundActions) {
			executor.execute(action);

			// publish changes via bus to trigger text box

			// wait till finished / response received

			// publish changes via bus to trigger animations

			// wait till finished / repsonse received

			// go to next combat action
		}

		// clear round actions at the end of the turn
		battleState.roundActionsState.roundActions.clear();

		// transition back to input event handling / maybe not like this
		battleState.battlePhaseState.currentPhase = BattlePhase::PlayerTurn;
	}
};
} // namespace Engine::ECS