#pragma once
#include "ecs/components/combat/DeadComponent.hpp"
#include "ecs/components/combat/StatsComponent.hpp"
#include "ecs/world/World.hpp"

#include <iostream>

namespace Engine::ECS {
class DeathSystem {

  public:
	static void Update(SystemContext &context, const std::vector<Entity> &combatants)
	{
		for (const auto &combatant : combatants) {
			const auto &stats = context.GetComponent<StatsComponent>(combatant);
			if (stats.currHP == 0) {
				if (!context.HasComponent<DeadComponent>(combatant)) {
					context.EmplaceComponent<DeadComponent>(combatant);
				}
			} else {
				if (context.HasComponent<DeadComponent>(combatant)) {
					const bool removed = context.RemoveComponent<DeadComponent>(combatant);
					if (!removed) {
						std::cerr << "Failed to remove DeadComponent" << std::endl;
					}
				}
			}
		}
	}
};
} // namespace Engine::ECS