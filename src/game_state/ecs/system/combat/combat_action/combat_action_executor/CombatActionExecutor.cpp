#include "CombatActionExecutor.hpp"

#include "game_state/ecs/components/combat/StatsComponent.hpp"

void CombatActionExecutor::execute(CombatAction action)
{
	const auto &[actionEffects] = world.GetComponent<ActionEffectsComponent>(action.actionEntity);
	for (const auto &effect : actionEffects) {
		std::visit([this, &action](const auto &concreteEffect) { applyEffect(concreteEffect, action); }, effect);
	}
}

void CombatActionExecutor::applyEffect(const DamageEffect &effect, const CombatAction &action)
{
	const auto &actorStats = world.GetComponent<StatsComponent>(action.actionEntity);

	for (const auto target : action.targetEntities) {
		auto &targetStats = world.GetComponent<StatsComponent>(target);
	}
}

void CombatActionExecutor::applyEffect(const HealingEffect &effect, const CombatAction &action) {}
