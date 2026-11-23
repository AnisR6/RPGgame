#pragma once

#include "ecs/combat/components/DeadComponent.hpp"
#include "ecs/combat/components/StatsComponent.hpp"
#include "ecs/core/systems/SystemContext.hpp"

#include <iostream>
#include <vector>

namespace DeathRules {
    inline void UpdateDeadComponents(Engine::ECS::SystemContext& context,
                                     const std::vector<Engine::ECS::Entity>& combatants) {
        for (const auto& combatant : combatants) {
            const auto& stats = context.GetComponent<StatsComponent>(combatant);

            if (stats.currHP == 0) {
                if (!context.HasComponent<DeadComponent>(combatant)) {
                    context.EmplaceComponent<DeadComponent>(combatant);
                }
            } else if (context.HasComponent<DeadComponent>(combatant)) {
                if (!context.RemoveComponent<DeadComponent>(combatant)) {
                    std::cerr << "Failed to remove DeadComponent" << std::endl;
                }
            }
        }
    }
}
