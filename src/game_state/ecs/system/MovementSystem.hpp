
#pragma once

#include "game_state/ecs/entity/Entity.hpp"
#include "game_state/ecs/query/Query.hpp"
#include "game_state/ecs/system/ISystem.hpp"
#include "game_state/ecs/components/Position.hpp"
#include "game_state/ecs/components/Velocity.hpp"

namespace Engine::ECS {

    // ------------------------------------------------------------
    // MovementSystem
    // ------------------------------------------------------------

    class MovementSystem : public ISystem {
        public:
            // ------------------------------------------------------------
            // System Update
            // ------------------------------------------------------------

            void Update(SystemContext& context, float deltaTime) override {
                auto movementQuery = context.CreateQuery<Including<Position, Velocity>>();

                movementQuery.ForEach([deltaTime](Entity, Position& position, Velocity& velocity) {
                    position.x += velocity.x * deltaTime;
                    position.y += velocity.y * deltaTime;
                });
            }
    };

} // namespace Engine::ECS
