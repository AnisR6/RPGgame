#pragma once
#include "ecs/combat/systems/CombatAction.hpp"
#include "ecs/combat/components/ActionEffectsComponent.hpp"
#include "ecs/combat/components/StatsComponent.hpp"
#include "ecs/core/world/World.hpp"

class CombatActionExecutor {
    private:
        Engine::ECS::World& world;

        void ApplyEffect(const DamageEffect& effect, const CombatAction& action) const;
        void ApplyEffect(const HealingEffect& effect, const CombatAction& action) const;
        void ApplyEffect(const CostMPEffect& effect, const CombatAction& action) const;

        [[nodiscard]] StatsComponent& GetStats(Engine::ECS::Entity combatantEntity) const;

    public:
        explicit CombatActionExecutor(Engine::ECS::World& world)
            : world(world) {}
        void Execute(CombatAction action);
};
