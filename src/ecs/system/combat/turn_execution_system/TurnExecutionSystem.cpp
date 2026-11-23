#include "TurnExecutionSystem.hpp"

#include "ecs/system/combat/turn_order/TurnOrderUtils.hpp"

namespace Engine::ECS {
void TurnExecutionSystem::startDialog(const CombatAction &action)
{
	dialogFinished = false;
	executionState = ExecutionState::ShowDialog;
	battleMessageBus.publish(ShowBattleDialogRequest{.actorEntity = action.actorEntity,
	                                                 .actionEntity = action.actionEntity,
	                                                 .targetEntities = action.targetEntities});
}

void TurnExecutionSystem::startAnimation(const CombatAction &action)
{
	animationFinished = false;
	executionState = ExecutionState::PlayAnimation;
	battleMessageBus.publish(PlayBattleAnimationRequest{.actorEntity = action.actorEntity,
	                                                    .actionEntity = action.actionEntity,
	                                                    .targetEntities = action.targetEntities});
}

void TurnExecutionSystem::startNextAction(const SystemContext &context)
{
	auto &actions = battleState.roundActionsState.roundActions;

	while (currentActionIndex < actions.size()) {
		auto &action = actions.at(currentActionIndex);

		if (context.HasComponent<DeadComponent>(action.actorEntity)) {
			++currentActionIndex;
			continue;
		}

		if (!TargetResolver::retarget(context, battleState, action)) {
			++currentActionIndex;
			continue;
		}

		startDialog(action);
		return;
	}
	// No actions left
	finishTurn(context);
}

void TurnExecutionSystem::finishTurn(const SystemContext &context)
{
	battleState.roundActionsState.roundActions.clear();
	currentActionIndex = 0;
	dialogFinished = false;
	animationFinished = false;
	executionState = ExecutionState::Idle;

	const auto &turnOrder = battleState.turnOrderState;
	battleState.turnOrderState.currentActorIndex =
	    TurnOrderUtils::findNextLivingPartyMemberIndex(turnOrder.playerPartyEntities, 0, context);
	battleState.battlePhaseState.currentPhase = BattlePhase::PlayerTurn;

	battleMessageBus.publish(PlayerTurnStarted{});
}

// TODO: explicit handling via WonBattle / LostBattle Messages => initiating transition back to Overworld
void TurnExecutionSystem::finishBattle(const BattleOutcome outcome)
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
} // namespace Engine::ECS