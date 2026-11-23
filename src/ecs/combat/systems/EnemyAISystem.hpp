#pragma once
#include "ecs/combat/systems/BattleState.hpp"
#include "ecs/combat/components/DeadComponent.hpp"
#include "ecs/combat/components/EnemyAIComponent.hpp"
#include "ecs/combat/components/KnownActionsComponent.hpp"
#include "ecs/combat/components/StatsComponent.hpp"
#include "ecs/combat/components/TargetScopeComponent.hpp"
#include "ecs/core/systems/ISystem.hpp"
#include "ecs/combat/systems/CombatAction.hpp"

struct ScoredAction {
        CombatAction action;
        float value = 0.f;

        bool operator<(const ScoredAction& other) const { return this->value < other.value; }
};

class EnemyAISystem : public Engine::ECS::ISystem {
    private:
        BattleState& battleState;

        [[nodiscard]] std::vector<CombatAction>
        PossibleActions(const Engine::ECS::SystemContext& context, Engine::ECS::Entity actor,
                        const std::vector<Engine::ECS::Entity>& knownActions) const;

        float ValuateAction(Engine::ECS::SystemContext& context, const CombatAction& action,
                            const EnemyAIProfile& profile, const StatsComponent& actorStats);

        std::vector<ScoredAction>
        ValuateActions(Engine::ECS::SystemContext& context, Engine::ECS::Entity actor,
                       const EnemyAIProfile& profile,
                       const std::vector<Engine::ECS::Entity>& knownActions,
                       const StatsComponent& actorStats);

        CombatAction
        ChooseHighestValuatedAction(Engine::ECS::SystemContext& context, Engine::ECS::Entity actor,
                                    const EnemyAIProfile& profile,
                                    const std::vector<Engine::ECS::Entity>& knownActions,
                                    const StatsComponent& actorStats);

    public:
        explicit EnemyAISystem(BattleState& battleState)
            : battleState(battleState) {}

        void Update(Engine::ECS::SystemContext& context, float deltaTime) override {
            if (battleState.battlePhaseState.currentPhase != BattlePhase::EnemyAITurn) {
                return;
            }

            const auto query = context.CreateQuery<
                Engine::ECS::Including<EnemyAIComponent, KnownActionsComponent, StatsComponent>,
                Engine::ECS::Excluding<DeadComponent>>();
            query.ForEach([this, &context](const Engine::ECS::Entity entity,
                                           const EnemyAIComponent& ai,
                                           const KnownActionsComponent& knownActions,
                                           const StatsComponent& stats) {
                const auto bestAction = ChooseHighestValuatedAction(
                    context, entity, ai.profile, knownActions.knownActionEntities, stats);

                battleState.roundActionsState.roundActions.push_back(bestAction);
            });

            battleState.battlePhaseState.currentPhase = BattlePhase::ExecuteTurn;
        }
};
