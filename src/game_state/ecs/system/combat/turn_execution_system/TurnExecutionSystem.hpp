#pragma once
#include "../combat_action/combat_action_executor/CombatActionExecutor.hpp"
#include "BattleOutcomeEvaluator.hpp"
#include "application/bus/BattleMessageBus.hpp"
#include "game_state/ecs/system/ISystem.hpp"
#include "game_state/ecs/system/combat/battle_state/BattleState.hpp"

#include <algorithm>

namespace Engine::ECS {

enum class ExecutionState { Idle, ShowDialog, ExecuteAction, PlayAnimation, CheckBattleOutcome };

class TurnExecutionSystem : public ISystem {
  private:
	BattleState &battleState;
	BattleMessageBus &battleMessageBus;
	CombatActionExecutor &executor;
	ExecutionState executionState = ExecutionState::Idle;
	bool dialogFinished = false;
	bool animationFinished = false;
	std::size_t currentActionIndex = 0;

	void startDialog(const CombatAction &action);
	void startAnimation(const CombatAction &action);
	void finishTurn();
	void finishBattle(BattleOutcome outcome);

  public:
	TurnExecutionSystem(BattleState &battleState, BattleMessageBus &messageBus, CombatActionExecutor &executor)
	    : battleState(battleState), battleMessageBus(messageBus), executor(executor)
	{
		battleMessageBus.subscribe<ShowBattleDialogFinished>([this](const ShowBattleDialogFinished &) {
			if (executionState == ExecutionState::ShowDialog) {
				dialogFinished = true;
			}
		});
		battleMessageBus.subscribe<PlayBattleAnimationFinished>([this](const PlayBattleAnimationFinished &) {
			if (executionState == ExecutionState::PlayAnimation) {
				animationFinished = true;
			}
		});
	}

	void Update(SystemContext &context, float deltaTime) override
	{
		if (battleState.battlePhaseState.currentPhase != BattlePhase::ExecuteTurn) {
			return;
		}

		auto &actions = battleState.roundActionsState.roundActions;
		switch (executionState) {
		case ExecutionState::Idle: {
			std::ranges::stable_sort(actions, [&context](const CombatAction &a, const CombatAction &b) {
				const auto &statsA = context.GetComponent<StatsComponent>(a.actorEntity);
				const auto &statsB = context.GetComponent<StatsComponent>(b.actorEntity);
				return statsA.speed > statsB.speed;
			});
			currentActionIndex = 0;

			if (actions.empty()) {
				finishTurn();
				return;
			}

			startDialog(actions.at(currentActionIndex));
			return;
		}
		case ExecutionState::ShowDialog: {
			if (!dialogFinished) {
				return;
			}

			dialogFinished = false;
			executionState = ExecutionState::ExecuteAction;
			return;
		}
		case ExecutionState::ExecuteAction: {
			const auto &action = actions.at(currentActionIndex);

			executor.execute(action);
			startAnimation(action);
			return;
		}

		case ExecutionState::PlayAnimation: {
			if (!animationFinished) {
				return;
			}

			animationFinished = false;
			executionState = ExecutionState::CheckBattleOutcome;
			return;
		}
		case ExecutionState::CheckBattleOutcome: {
			const auto outcome = BattleOutcomeEvaluator::evaluate(context, battleState);
			if (outcome != BattleOutcome::Ongoing) {
				finishBattle(outcome);
				return;
			}

			++currentActionIndex;
			if (currentActionIndex >= actions.size()) {
				finishTurn();
				return;
			}
			startDialog(actions[currentActionIndex]);
			return;
		}
		}
	}
};
} // namespace Engine::ECS