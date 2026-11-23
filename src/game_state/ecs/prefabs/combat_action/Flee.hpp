#pragma once
#include "game_state/ecs/components/NameComponent.hpp"
#include "game_state/ecs/components/combat/ActionComponent.hpp"
#include "game_state/ecs/components/combat/FleeComponent.hpp"
#include "game_state/ecs/world/World.hpp"

namespace Prefabs {

struct FleeBlueprint {
	ActionComponent actionComponent;
	FleeComponent fleeComponent;
	NameComponent nameComponent;
	TargetScopeComponent targetScopeComponent{TargetScope::SELF};
};

inline Engine::ECS::Entity createFlee(Engine::ECS::World &world, FleeBlueprint &&blueprint)
{
	const auto entity = world.CreateEntity();
	world.EmplaceComponent<ActionComponent>(entity, blueprint.actionComponent);
	world.EmplaceComponent<FleeComponent>(entity, blueprint.fleeComponent);
	world.EmplaceComponent<NameComponent>(entity, blueprint.nameComponent);
	world.EmplaceComponent<TargetScopeComponent>(entity, blueprint.targetScopeComponent);
	return entity;
}
} // namespace Prefabs
