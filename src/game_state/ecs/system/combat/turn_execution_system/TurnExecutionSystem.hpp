#pragma once
#include "game_state/ecs/system/ISystem.hpp"
#include "game_state/ecs/system/combat/BattleSystem.hpp"

namespace Engine::ECS {
class TurnExecutionSystem : public ISystem {
  public:
	void Update(SystemContext &context, float deltaTime) override
	{
		const auto query = context.CreateQuery<Including<BattlePhaseComponent>>();
		query.ForEach([](Entity entity, BattlePhaseComponent &battleState) {
			if (battleState.currentPhase != BattlePhase::ExecuteTurn) {
				return;
			}

			if () {
				battleState.currentPhase = BattlePhase::WinBattle;
			} else if () {
				battleState.currentPhase = BattlePhase::LoseBattle;
			} else {
				battleState.currentPhase = BattlePhase::PlayerTurn;
			}
		});
	}
};
} // namespace Engine::ECS