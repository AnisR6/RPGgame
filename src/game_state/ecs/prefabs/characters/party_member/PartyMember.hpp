#pragma once
#include "game_state/ecs/components/NameComponent.hpp"
#include "game_state/ecs/components/PartyMemberComponent.hpp"
#include "game_state/ecs/components/SpriteComponent.hpp"
#include "game_state/ecs/entity/Entity.hpp"
#include "game_state/ecs/world/World.hpp"

namespace Prefabs {
struct PartyMemberBlueprint {
	NameComponent nameComponent;
	PartyMemberComponent partyMemberComponent;
	StatsComponent statsComponent;
	KnownActionsComponent knownActionsComponent;
	SpriteComponent spriteComponent;
};

inline Engine::ECS::Entity createPartyMember(Engine::ECS::World &world, PartyMemberBlueprint &&blueprint)
{
	const auto entity = world.CreateEntity();
	world.EmplaceComponent<NameComponent>(entity, std::move(blueprint.nameComponent));
	world.EmplaceComponent<PartyMemberComponent>(entity, blueprint.partyMemberComponent);
	world.EmplaceComponent<StatsComponent>(entity, blueprint.statsComponent);
	world.EmplaceComponent<KnownActionsComponent>(entity, blueprint.knownActionsComponent);
	world.EmplaceComponent<SpriteComponent>(entity, std::move(blueprint.spriteComponent));
	return entity;
}
} // namespace Prefabs