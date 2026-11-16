#pragma once
#include "../../../components/combat/StatsComponent.hpp"
#include "game_state/ecs/components/CombatSkillComponent.hpp"
#include "game_state/ecs/components/NameComponent.hpp"
#include "game_state/ecs/components/PartyMemberComponent.hpp"
#include "game_state/ecs/components/SpellComponent.hpp"
#include "game_state/ecs/components/SpriteComponent.hpp"
#include "game_state/ecs/entity/Entity.hpp"
#include "game_state/ecs/world/World.hpp"

namespace Prefabs {
struct PartyMemberBlueprint {
	NameComponent nameComponent;
	PartyMemberComponent partyMemberComponent;
	StatsComponent statsComponent;
	SpellComponent spellComponent;
	CombatSkillComponent combatSkillComponent;
	SpriteComponent spriteComponent;
};

inline Engine::ECS::Entity createPartyMember(Engine::ECS::World &world, PartyMemberBlueprint &&blueprint)
{
	const auto entity = world.CreateEntity();
	world.EmplaceComponent<NameComponent>(entity, std::move(blueprint.nameComponent));
	world.EmplaceComponent<PartyMemberComponent>(entity, blueprint.partyMemberComponent);
	world.EmplaceComponent<StatsComponent>(entity, blueprint.statsComponent);
	world.EmplaceComponent<SpellComponent>(entity, std::move(blueprint.spellComponent));
	world.EmplaceComponent<CombatSkillComponent>(entity, std::move(blueprint.combatSkillComponent));
	world.EmplaceComponent<SpriteComponent>(entity, std::move(blueprint.spriteComponent));
	return entity;
}
} // namespace Prefabs