#pragma once
#include "../../../components/combat/StatsComponent.hpp"
#include "ecs/components/overworld/EnemyComponent.hpp"
#include "ecs/components/overworld/NameComponent.hpp"
#include "ecs/components/overworld/SpriteComponent.hpp"
#include "ecs/components/combat/EnemyAIComponent.hpp"
#include "ecs/components/combat/KnownActionsComponent.hpp"
#include "ecs/entity/Entity.hpp"
#include "ecs/world/World.hpp"

namespace Prefabs {

struct EnemyBlueprint {
	NameComponent nameComponent;
	EnemyComponent enemyComponent;
	StatsComponent statsComponent;
	KnownActionsComponent knownActionsComponent;
	EnemyAIComponent enemyAIComponent{};
	SpriteComponent spriteComponent;
};

inline Engine::ECS::Entity createEnemy(Engine::ECS::World &world, EnemyBlueprint &&blueprint)
{
	const auto entity = world.CreateEntity();
	world.EmplaceComponent<NameComponent>(entity, std::move(blueprint.nameComponent));
	world.EmplaceComponent<StatsComponent>(entity, blueprint.statsComponent);
	world.EmplaceComponent<EnemyComponent>(entity, blueprint.enemyComponent);
	world.EmplaceComponent<KnownActionsComponent>(entity, blueprint.knownActionsComponent);
	world.EmplaceComponent<EnemyAIComponent>(entity, blueprint.enemyAIComponent);
	world.EmplaceComponent<SpriteComponent>(entity, std::move(blueprint.spriteComponent));
	return entity;
}

} // namespace Prefabs