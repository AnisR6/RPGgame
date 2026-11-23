#include "ecs/combat/systems/TurnExecutionSystem.hpp"

#include "ecs/combat/systems/TurnOrder.hpp"

void TurnExecutionSystem::StartDialog(const CombatAction& action) {
    dialogFinished = false;
    executionState = ExecutionState::ShowDialog;
    battleMessageBus.Publish(ShowBattleDialogRequest{.actorEntity = action.actorEntity,
                                                     .actionEntity = action.actionEntity,
                                                     .targetEntities = action.targetEntities});
}

void TurnExecutionSystem::StartAnimation(const CombatAction& action) {
    animationFinished = false;
    executionState = ExecutionState::PlayAnimation;
    battleMessageBus.Publish(PlayBattleAnimationRequest{.actorEntity = action.actorEntity,
                                                        .actionEntity = action.actionEntity,
                                                        .targetEntities = action.targetEntities});
}

void TurnExecutionSystem::StartNextAction(const Engine::ECS::SystemContext& context) {
    auto& actions = battleState.roundActionsState.roundActions;

    while (currentActionIndex < actions.size()) {
        auto& action = actions.at(currentActionIndex);

        if (context.HasComponent<DeadComponent>(action.actorEntity)) {
            ++currentActionIndex;
            continue;
        }

        if (!TargetResolver::Retarget(context, battleState, action)) {
            ++currentActionIndex;
            continue;
        }

        StartDialog(action);
        return;
    }
    FinishTurn(context);
}

void TurnExecutionSystem::FinishTurn(const Engine::ECS::SystemContext& context) {
    battleState.roundActionsState.roundActions.clear();
    currentActionIndex = 0;
    dialogFinished = false;
    animationFinished = false;
    executionState = ExecutionState::Idle;

    const auto& turnOrder = battleState.turnOrderState;
    battleState.turnOrderState.currentActorIndex =
        TurnOrder::FindNextLivingPartyMemberIndex(turnOrder.playerPartyEntities, 0, context);
    battleState.battlePhaseState.currentPhase = BattlePhase::PlayerTurn;

    battleMessageBus.Publish(PlayerTurnStarted{});
}

void TurnExecutionSystem::FinishBattle(const BattleOutcome outcome) {
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
