#pragma once
#include "game_state/ecs/components/NameComponent.hpp"
#include "game_state/ecs/components/combat/ActionComponent.hpp"
#include "game_state/ecs/components/combat/ActionEffectsComponent.hpp"
#include "game_state/ecs/components/combat/CombatSkillComponent.hpp"
#include "game_state/ecs/components/combat/TargetScopeComponent.hpp"
#include "game_state/ecs/world/World.hpp"

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
