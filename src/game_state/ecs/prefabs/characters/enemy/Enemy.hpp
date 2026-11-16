#pragma once
#include "../../../components/combat/StatsComponent.hpp"
#include "game_state/ecs/components/CombatSkillComponent.hpp"
#include "game_state/ecs/components/EnemyComponent.hpp"
#include "game_state/ecs/components/NameComponent.hpp"
#include "game_state/ecs/components/SpellComponent.hpp"
#include "game_state/ecs/components/SpriteComponent.hpp"
#include "game_state/ecs/components/combat/EnemyAIComponent.hpp"
#include "game_state/ecs/entity/Entity.hpp"
#include "game_state/ecs/world/World.hpp"

namespace Prefabs {

struct EnemyBlueprint {
	NameComponent nameComponent;
	EnemyComponent enemyComponent;
	StatsComponent statsComponent;
	SpellComponent spellComponent;
	CombatSkillComponent combatSkillComponent;
	EnemyAIComponent enemyAIComponent{};
	SpriteComponent spriteComponent;
};

inline Engine::ECS::Entity createEnemy(Engine::ECS::World &world, EnemyBlueprint &&blueprint)
{
	const auto entity = world.CreateEntity();
	world.EmplaceComponent<NameComponent>(entity, std::move(blueprint.nameComponent));
	world.EmplaceComponent<StatsComponent>(entity, blueprint.statsComponent);
	world.EmplaceComponent<EnemyComponent>(entity, blueprint.enemyComponent);
	world.EmplaceComponent<SpellComponent>(entity, std::move(blueprint.spellComponent));
	world.EmplaceComponent<CombatSkillComponent>(entity, std::move(blueprint.combatSkillComponent));
	world.EmplaceComponent<EnemyAIComponent>(entity, blueprint.enemyAIComponent);
	world.EmplaceComponent<SpriteComponent>(entity, std::move(blueprint.spriteComponent));
	return entity;
}

} // namespace Prefabs