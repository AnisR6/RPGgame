#pragma once
#include "game_state/ecs/system/ISystem.hpp"
#include "game_state/ecs/system/combat/BattleSystem.hpp"

namespace Engine::ECS {
class EnemyAiSystem : public ISystem {
  public:
	void Update(SystemContext &context, float deltaTime) override
	{
		const auto query = context.CreateQuery<Including<BattlePhaseComponent>>();
		query.ForEach([](Entity entity, BattlePhaseComponent &battleState) {
			if (battleState.currentPhase != BattlePhase::EnemyAITurn) {
				return;
			}

			battleState.currentPhase = BattlePhase::ExecuteTurn;
		});
	}
};
} // namespace Engine::ECS