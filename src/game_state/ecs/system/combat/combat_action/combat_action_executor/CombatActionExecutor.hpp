#pragma once
#include "../CombatAction.hpp"
#include "game_state/ecs/components/combat/ActionEffectsComponent.hpp"
#include "game_state/ecs/world/World.hpp"

class CombatActionExecutor {
  private:
	Engine::ECS::World &world;

	void applyEffect(const DamageEffect &effect, const CombatAction &action);
	void applyEffect(const HealingEffect &effect, const CombatAction &action);

	static int computePhysicalDamage(int actor_atk, int target_def);
	static int computeMagicalDamage(int actor_mag, int target_res);

  public:
	explicit CombatActionExecutor(Engine::ECS::World &world) : world(world) {}
	void execute(CombatAction action);
};