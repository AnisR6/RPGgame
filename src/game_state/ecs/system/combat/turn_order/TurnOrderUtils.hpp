#pragma once
#include "game_state/ecs/components/combat/DeadComponent.hpp"
#include "game_state/ecs/entity/Entity.hpp"

#include <cstddef>
#include <vector>

namespace Engine::ECS {
class TurnOrderUtils {

  public:
	template <typename Context>
	static std::size_t findNextLivingPartyMemberIndex(const std::vector<Entity> &party, const std::size_t startIndex,
	                                                  const Context &context)
	{
		for (std::size_t i = startIndex; i < party.size(); i++) {
			if (!context.template HasComponent<DeadComponent>(party.at(i))) {
				return i;
			}
		}
		return party.size();
	}
};
} // namespace Engine::ECS