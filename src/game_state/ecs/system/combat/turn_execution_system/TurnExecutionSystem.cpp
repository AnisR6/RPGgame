#include "TurnExecutionSystem.hpp"

void Engine::ECS::TurnExecutionSystem::startDialog(const CombatAction &action)
{
	dialogFinished = false;
	executionState = ExecutionState::ShowDialog;
	battleMessageBus.publish(ShowBattleDialogRequest{.actorEntity = action.actorEntity,
	                                                 .actionEntity = action.actionEntity,
	                                                 .targetEntities = action.targetEntities});
}

void Engine::ECS::TurnExecutionSystem::startAnimation(const CombatAction &action)
{
	animationFinished = false;
	executionState = ExecutionState::PlayAnimation;
	battleMessageBus.publish(PlayBattleAnimationRequest{.actorEntity = action.actorEntity,
	                                                    .actionEntity = action.actionEntity,
	                                                    .targetEntities = action.targetEntities});
}

void Engine::ECS::TurnExecutionSystem::finishTurn()
{
	battleState.roundActionsState.roundActions.clear();
	currentActionIndex = 0;
	dialogFinished = false;
	animationFinished = false;
	executionState = ExecutionState::Idle;

	battleState.battlePhaseState.currentPhase = BattlePhase::PlayerTurn;
}

// TODO: explicit handling via WonBattle / LostBattle Messages
void Engine::ECS::TurnExecutionSystem::finishBattle(const BattleOutcome outcome)
{
	if (outcome == BattleOutcome::Ongoing) {
		return;
	}

	if (outcome == BattleOutcome::Victory) {
		battleState.battlePhaseState.currentPhase = BattlePhase::WonBattle;
	} else if (outcome == BattleOutcome::Defeat) {
		battleState.battlePhaseState.currentPhase = BattlePhase::LostBattle;
	}

	battleState.roundActionsState.roundActions.clear();
	currentActionIndex = 0;
	dialogFinished = false;
	animationFinished = false;
	executionState = ExecutionState::Idle;
}