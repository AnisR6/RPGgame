#include "CombatActionExecutor.hpp"

#include "ecs/components/combat/StatsComponent.hpp"

void CombatActionExecutor::execute(CombatAction action)
{
	const auto &[actionEffects] = world.GetComponent<ActionEffectsComponent>(action.actionEntity);
	for (const auto &effect : actionEffects) {
		std::visit([this, &action](const auto &concreteEffect) { applyEffect(concreteEffect, action); }, effect);
	}
}

void CombatActionExecutor::applyEffect(const DamageEffect &effect, const CombatAction &action) const
{
	const auto &actorStats = getStats(action.actorEntity);

	for (const auto target : action.targetEntities) {
		auto &targetStats = getStats(target);
		const auto damage = effect.magical ? effect.power + actorStats.magic - targetStats.resistance
		                                   : effect.power + actorStats.attack - targetStats.defense;
		targetStats.currHP = std::max(0, targetStats.currHP - std::max(0, damage));
	}
}

void CombatActionExecutor::applyEffect(const HealingEffect &effect, const CombatAction &action) const
{
	const auto &actorStats = getStats(action.actorEntity);

	for (const auto target : action.targetEntities) {
		auto &targetStats = getStats(target);
		const auto healingPower = effect.power + actorStats.magic;
		targetStats.currHP = std::min(targetStats.maxHP, targetStats.currHP + healingPower);
	}
}

void CombatActionExecutor::applyEffect(const CostMPEffect &effect, const CombatAction &action) const
{
	auto &actorStats = getStats(action.actorEntity);
	actorStats.currMP = std::max(0, actorStats.currMP - effect.mpCost);
}

StatsComponent &CombatActionExecutor::getStats(const Engine::ECS::Entity combatantEntity) const
{
	return world.GetComponent<StatsComponent>(combatantEntity);
}
