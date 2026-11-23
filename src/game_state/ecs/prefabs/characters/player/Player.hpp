#pragma once

#include <SFML/Graphics.hpp>
#include <string>

#include "../../../components/combat/StatsComponent.hpp"
#include "game_state/ecs/components/CollisionComponent.hpp"
#include "game_state/ecs/components/MovementComponent.hpp"
#include "game_state/ecs/components/NameComponent.hpp"
#include "game_state/ecs/components/PartyMemberComponent.hpp"
#include "game_state/ecs/components/PositionComponent.hpp"
#include "game_state/ecs/components/SpriteComponent.hpp"
#include "game_state/ecs/components/combat/KnownActionsComponent.hpp"
#include "game_state/ecs/entity/Entity.hpp"
#include "game_state/ecs/world/World.hpp"

namespace Prefabs {
struct PlayerBlueprint {
	NameComponent nameComponent;
	PositionComponent positionComponent;
	CollisionComponent collisionComponent;
	MovementComponent movementComponent;
	SpriteComponent spriteComponent;
	PartyMemberComponent partyMemberComponent;
	StatsComponent statsComponent;
	KnownActionsComponent knownActionsComponent;
};

inline Engine::ECS::Entity createPlayer(Engine::ECS::World &world, PlayerBlueprint &&blueprint)
{
	const auto entity = world.CreateEntity();
	world.EmplaceComponent<NameComponent>(entity, std::move(blueprint.nameComponent));
	world.EmplaceComponent<PositionComponent>(entity, blueprint.positionComponent);
	world.EmplaceComponent<CollisionComponent>(entity, blueprint.collisionComponent);
	world.EmplaceComponent<MovementComponent>(entity, blueprint.movementComponent);
	world.EmplaceComponent<SpriteComponent>(entity, std::move(blueprint.spriteComponent));
	world.EmplaceComponent<PartyMemberComponent>(entity, blueprint.partyMemberComponent);
	world.EmplaceComponent<StatsComponent>(entity, blueprint.statsComponent);
	world.EmplaceComponent<KnownActionsComponent>(entity, blueprint.knownActionsComponent);
	return entity;
}

class Player {
  public:
	Player(sf::Vector2f startPosition, const std::string &spriteSheetPath);

	// The player's data, as components
	PositionComponent position;
	CollisionComponent collision;
	MovementComponent movement;
	SpriteComponent sprite;
};
} // namespace Prefabs