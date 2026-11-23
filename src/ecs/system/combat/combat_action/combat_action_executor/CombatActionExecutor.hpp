#pragma once
#include "../CombatAction.hpp"
#include "ecs/components/combat/ActionEffectsComponent.hpp"
#include "ecs/components/combat/StatsComponent.hpp"
#include "ecs/world/World.hpp"

class CombatActionExecutor {
  private:
	Engine::ECS::World &world;

	void applyEffect(const DamageEffect &effect, const CombatAction &action) const;
	void applyEffect(const HealingEffect &effect, const CombatAction &action) const;
	void applyEffect(const CostMPEffect &effect, const CombatAction &action) const;

	[[nodiscard]] StatsComponent &getStats(Engine::ECS::Entity combatantEntity) const;

  public:
	explicit CombatActionExecutor(Engine::ECS::World &world) : world(world) {}
	// TODO: should return a CombatActionResult which contains additional info for dialog
	void execute(CombatAction action);
};