#pragma once
#include "ecs/combat/systems/CombatActionExecutor.hpp"
#include "ecs/combat/systems/BattleOutcomeEvaluator.hpp"
#include "application/combat/bus/BattleMessageBus.hpp"
#include "ecs/combat/components/DeadComponent.hpp"
#include "ecs/core/systems/ISystem.hpp"
#include "ecs/combat/systems/BattleState.hpp"
#include "ecs/combat/systems/DeathRules.hpp"
#include "ecs/combat/systems/TargetResolver.hpp"

#include <algorithm>

enum class ExecutionState { Idle, ShowDialog, ExecuteAction, PlayAnimation, CheckBattleOutcome };

class TurnExecutionSystem : public Engine::ECS::ISystem {
    private:
        BattleState& battleState;
        BattleMessageBus& battleMessageBus;
        CombatActionExecutor& executor;
        ExecutionState executionState = ExecutionState::Idle;
        bool dialogFinished = false;
        bool animationFinished = false;
        std::size_t currentActionIndex = 0;

        void StartNextAction(const Engine::ECS::SystemContext& context);
        void StartDialog(const CombatAction& action);
        void StartAnimation(const CombatAction& action);
        void FinishTurn(const Engine::ECS::SystemContext& context);
        void FinishBattle(BattleOutcome outcome);

    public:
        TurnExecutionSystem(BattleState& battleState, BattleMessageBus& messageBus,
                            CombatActionExecutor& executor)
            : battleState(battleState),
              battleMessageBus(messageBus),
              executor(executor) {
            battleMessageBus.Subscribe<ShowBattleDialogFinished>(
                [this](const ShowBattleDialogFinished&) {
                    if (executionState == ExecutionState::ShowDialog) {
                        dialogFinished = true;
                    }
                });
            battleMessageBus.Subscribe<PlayBattleAnimationFinished>(
                [this](const PlayBattleAnimationFinished&) {
                    if (executionState == ExecutionState::PlayAnimation) {
                        animationFinished = true;
                    }
                });
        }

        void Update(Engine::ECS::SystemContext& context, float deltaTime) override {
            if (battleState.battlePhaseState.currentPhase != BattlePhase::ExecuteTurn) {
                return;
            }

            auto& actions = battleState.roundActionsState.roundActions;
            switch (executionState) {
            case ExecutionState::Idle: {
                std::ranges::stable_sort(
                    actions, [&context](const CombatAction& a, const CombatAction& b) {
                        const auto& statsA = context.GetComponent<StatsComponent>(a.actorEntity);
                        const auto& statsB = context.GetComponent<StatsComponent>(b.actorEntity);
                        return statsA.speed > statsB.speed;
                    });

                currentActionIndex = 0;
                StartNextAction(context);
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
                const CombatAction& action = actions.at(currentActionIndex);

                executor.Execute(action);

                const auto& partyMembers = battleState.turnOrderState.playerPartyEntities;
                const auto& enemies = battleState.turnOrderState.enemyEntities;
                std::vector<Engine::ECS::Entity> combatants;
                combatants.reserve(partyMembers.size() + enemies.size());
                combatants.insert(combatants.end(), partyMembers.cbegin(), partyMembers.cend());
                combatants.insert(combatants.end(), enemies.cbegin(), enemies.cend());

                DeathRules::UpdateDeadComponents(context, combatants);

                StartAnimation(action);
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
                const auto outcome = BattleOutcomeEvaluator::Evaluate(context, battleState);
                if (outcome != BattleOutcome::Ongoing) {
                    FinishBattle(outcome);
                    return;
                }

                ++currentActionIndex;
                StartNextAction(context);
                return;
            }
            }
        }
};
