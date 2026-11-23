#pragma once

#include "ecs/combat/components/DeadComponent.hpp"
#include "ecs/core/entity/Entity.hpp"

#include <cstddef>
#include <vector>

namespace TurnOrder {
    template <typename Context>
    std::size_t FindNextLivingPartyMemberIndex(const std::vector<Engine::ECS::Entity>& party,
                                               const std::size_t startIndex,
                                               const Context& context) {
        for (std::size_t index = startIndex; index < party.size(); ++index) {
            if (!context.template HasComponent<DeadComponent>(party.at(index))) {
                return index;
            }
        }

        return party.size();
    }
}
