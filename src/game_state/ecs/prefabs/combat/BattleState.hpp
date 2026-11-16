#pragma once
#include "game_state/ecs/components/combat/BattlePhaseComponent.hpp"
#include "game_state/ecs/components/combat/RoundActionsComponent.hpp"
#include "game_state/ecs/components/combat/TurnOrderComponent.hpp"
#include "game_state/ecs/entity/Entity.hpp"
#include "game_state/ecs/world/World.hpp"

namespace Prefabs {
struct BattleStateBlueprint {
	BattlePhaseComponent battlePhaseComponent;
	TurnOrderComponent turnOrderComponent;
	RoundActionsComponent roundActionsComponent;
};

inline Engine::ECS::Entity createBattleState(Engine::ECS::World &world, BattleStateBlueprint &&blueprint)
{
	const auto entity = world.CreateEntity();
	world.EmplaceComponent<BattlePhaseComponent>(entity, blueprint.battlePhaseComponent);
	world.EmplaceComponent<TurnOrderComponent>(entity, std::move(blueprint.turnOrderComponent));
	world.EmplaceComponent<RoundActionsComponent>(entity, std::move(blueprint.roundActionsComponent));
	return entity;
}
} // namespace Prefabs