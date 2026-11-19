#pragma once
#include "game_state/ecs/components/NameComponent.hpp"
#include "game_state/ecs/components/combat/ActionComponent.hpp"
#include "game_state/ecs/components/combat/ActionEffectsComponent.hpp"
#include "game_state/ecs/components/combat/FlightComponent.hpp"
#include "game_state/ecs/components/combat/SpellComponent.hpp"
#include "game_state/ecs/components/combat/TargetScopeComponent.hpp"
#include "game_state/ecs/world/World.hpp"

namespace Prefabs {

struct SpellBlueprint {
	ActionComponent actionComponent;
	FlightComponent flightComponent;
	NameComponent nameComponent;
};

inline Engine::ECS::Entity createSpell(Engine::ECS::World &world, SpellBlueprint &&blueprint)
{
	const auto entity = world.CreateEntity();
	world.EmplaceComponent<ActionComponent>(entity, blueprint.actionComponent);
	world.EmplaceComponent<FlightComponent>(entity, blueprint.flightComponent);
	world.EmplaceComponent<NameComponent>(entity, blueprint.nameComponent);
	return entity;
}
} // namespace Prefabs
