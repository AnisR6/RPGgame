#pragma once

#include "game_state/ecs/components/CollisionComponent.hpp"
#include "game_state/ecs/system/ISystem.hpp"

#include <SFML/System/Vector2.hpp>

namespace Engine::ECS {

    class CollisionSystem : public ISystem {
        public:
            void Update(SystemContext& context, float deltaTime) override;

            // Would this collision box touch a solid tile? Outside the map counts as solid.
            [[nodiscard]]
            static bool isBlocked(SystemContext& context, sf::Vector2f boxPosition,
                                  const CollisionComponent& collision);
    };

} // namespace Engine::ECS
