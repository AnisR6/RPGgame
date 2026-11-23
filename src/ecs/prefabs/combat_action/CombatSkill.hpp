#pragma once
#include "ecs/components/overworld/NameComponent.hpp"
#include "ecs/components/combat/ActionComponent.hpp"
#include "ecs/components/combat/ActionEffectsComponent.hpp"
#include "ecs/components/combat/CombatSkillComponent.hpp"
#include "ecs/components/combat/TargetScopeComponent.hpp"
#include "ecs/world/World.hpp"

namespace Prefabs {

struct CombatSkillBlueprint {
	ActionComponent actionComponent;
	CombatSkillComponent combatSkillComponent;
	NameComponent nameComponent;
	TargetScopeComponent targetScopeComponent;
	ActionEffectsComponent actionEffectsComponent;
};

inline Engine::ECS::Entity createCombatSkill(Engine::ECS::World &world, CombatSkillBlueprint &&blueprint)
{
	const auto entity = world.CreateEntity();
	world.EmplaceComponent<ActionComponent>(entity, blueprint.actionComponent);
	world.EmplaceComponent<CombatSkillComponent>(entity, blueprint.combatSkillComponent);
	world.EmplaceComponent<NameComponent>(entity, blueprint.nameComponent);
	world.EmplaceComponent<TargetScopeComponent>(entity, blueprint.targetScopeComponent);
	world.EmplaceComponent<ActionEffectsComponent>(entity, blueprint.actionEffectsComponent);
	return entity;
}
} // namespace Prefabs
