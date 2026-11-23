#include "ecs/combat/systems/CombatActionExecutor.hpp"

#include "ecs/combat/components/StatsComponent.hpp"

void CombatActionExecutor::Execute(CombatAction action) {
    const auto& [actionEffects] = world.GetComponent<ActionEffectsComponent>(action.actionEntity);
    for (const auto& effect : actionEffects) {
        std::visit(
            [this, &action](const auto& concreteEffect) { ApplyEffect(concreteEffect, action); },
            effect);
    }
}

void CombatActionExecutor::ApplyEffect(const DamageEffect& effect,
                                       const CombatAction& action) const {
    const auto& actorStats = GetStats(action.actorEntity);

    for (const auto target : action.targetEntities) {
        auto& targetStats = GetStats(target);
        const auto damage = effect.magical
                                ? effect.power + actorStats.magic - targetStats.resistance
                                : effect.power + actorStats.attack - targetStats.defense;
        targetStats.currHP = std::max(0, targetStats.currHP - std::max(0, damage));
    }
}

void CombatActionExecutor::ApplyEffect(const HealingEffect& effect,
                                       const CombatAction& action) const {
    const auto& actorStats = GetStats(action.actorEntity);

    for (const auto target : action.targetEntities) {
        auto& targetStats = GetStats(target);
        const auto healingPower = effect.power + actorStats.magic;
        targetStats.currHP = std::min(targetStats.maxHP, targetStats.currHP + healingPower);
    }
}

void CombatActionExecutor::ApplyEffect(const CostMPEffect& effect,
                                       const CombatAction& action) const {
    auto& actorStats = GetStats(action.actorEntity);
    actorStats.currMP = std::max(0, actorStats.currMP - effect.mpCost);
}

StatsComponent& CombatActionExecutor::GetStats(const Engine::ECS::Entity combatantEntity) const {
    return world.GetComponent<StatsComponent>(combatantEntity);
}
