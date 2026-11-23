#pragma once
#include "ecs/components/overworld/NameComponent.hpp"
#include "ecs/components/combat/ActionComponent.hpp"
#include "ecs/components/combat/ActionEffectsComponent.hpp"
#include "ecs/components/combat/BaseAttackComponent.hpp"
#include "ecs/components/combat/TargetScopeComponent.hpp"
#include "ecs/world/World.hpp"

namespace Prefabs {

struct BaseAttackBlueprint {
	ActionComponent actionComponent;
	BaseAttackComponent baseAttackComponent;
	NameComponent nameComponent;
	TargetScopeComponent targetScopeComponent;
	ActionEffectsComponent actionEffectsComponent;
};

inline Engine::ECS::Entity createBaseAttack(Engine::ECS::World &world, BaseAttackBlueprint &&blueprint)
{
	const auto entity = world.CreateEntity();
	world.EmplaceComponent<ActionComponent>(entity, blueprint.actionComponent);
	world.EmplaceComponent<BaseAttackComponent>(entity, blueprint.baseAttackComponent);
	world.EmplaceComponent<NameComponent>(entity, blueprint.nameComponent);
	world.EmplaceComponent<TargetScopeComponent>(entity, blueprint.targetScopeComponent);
	world.EmplaceComponent<ActionEffectsComponent>(entity, blueprint.actionEffectsComponent);
	return entity;
}
} // namespace Prefabs
