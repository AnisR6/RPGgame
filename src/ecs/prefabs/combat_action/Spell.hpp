#pragma once
#include "ecs/components/overworld/NameComponent.hpp"
#include "ecs/components/combat/ActionComponent.hpp"
#include "ecs/components/combat/ActionEffectsComponent.hpp"
#include "ecs/components/combat/SpellComponent.hpp"
#include "ecs/components/combat/TargetScopeComponent.hpp"
#include "ecs/world/World.hpp"

namespace Prefabs {

struct SpellBlueprint {
	ActionComponent actionComponent;
	SpellComponent spellComponent;
	NameComponent nameComponent;
	TargetScopeComponent targetScopeComponent;
	ActionEffectsComponent actionEffectsComponent;
};

inline Engine::ECS::Entity createSpell(Engine::ECS::World &world, SpellBlueprint &&blueprint)
{
	const auto entity = world.CreateEntity();
	world.EmplaceComponent<ActionComponent>(entity, blueprint.actionComponent);
	world.EmplaceComponent<SpellComponent>(entity, blueprint.spellComponent);
	world.EmplaceComponent<NameComponent>(entity, blueprint.nameComponent);
	world.EmplaceComponent<TargetScopeComponent>(entity, blueprint.targetScopeComponent);
	world.EmplaceComponent<ActionEffectsComponent>(entity, blueprint.actionEffectsComponent);
	return entity;
}
} // namespace Prefabs