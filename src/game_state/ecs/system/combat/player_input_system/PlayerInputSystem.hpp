#pragma once
#include "game_state/ecs/components/combat/BattlePhaseComponent.hpp"
#include "game_state/ecs/system/ISystem.hpp"
#include "game_state/ecs/system/combat/BattleSystem.hpp"

namespace Engine::ECS {
class PlayerInputSystem : public ISystem {
  public:
	void Update(SystemContext &context, float deltaTime) override
	{
		const auto query = context.CreateQuery<Including<BattlePhaseComponent>>();
		const auto battleStateEntity = query.TryGetSingleEntity();
		if (!battleStateEntity.has_value()) {
			return;
		}

		if (auto &[currentPhase] = context.GetComponent<BattlePhaseComponent>(battleStateEntity.value());
		    currentPhase != BattlePhase::PlayerTurn) {
			return;
		}

		query.ForEach([](Entity entity, BattlePhaseComponent &battleState) {
			if (battleState.currentPhase != BattlePhase::PlayerTurn) {
				return;
			};

			battleState.currentPhase = BattlePhase::EnemyAITurn;
		});
	}
};
} // namespace Engine::ECS