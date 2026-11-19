#pragma once
#include "game_state/ecs/components/combat/TargetScopeComponent.hpp"
#include "game_state/ecs/entity/Entity.hpp"

#include <vector>

namespace Engine::ECS::TargetResolver {
using TargetGroup = std::vector<Entity>;

inline std::vector<TargetGroup> resolve(const TargetScope targetScope, Entity actor, const std::vector<Entity> &allies,
                                        const std::vector<Entity> &enemies)
{
	switch (targetScope) {
	case TargetScope::SELF:
		return {{actor}};

	case TargetScope::SINGLE_ALLIE: {
		std::vector<TargetGroup> result;
		result.reserve(allies.size());

		for (auto ally : allies) {
			result.push_back({ally});
		}
		return result;
	}
	case TargetScope::SINGLE_ENEMY: {
		std::vector<TargetGroup> result;
		result.reserve(enemies.size());

		for (auto enemy : enemies) {
			result.push_back({enemy});
		}
		return result;
	}
	case TargetScope::ALL_ALLIES: {
		return allies.empty() ? std::vector<TargetGroup>{} : std::vector<TargetGroup>{allies};
	}
	case TargetScope::ALL_ENEMIES: {
		return enemies.empty() ? std::vector<TargetGroup>{} : std::vector<TargetGroup>{enemies};
	}
	}

	return {};
}
} // namespace Engine::ECS::TargetResolver