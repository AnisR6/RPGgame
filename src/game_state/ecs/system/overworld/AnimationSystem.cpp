#include "game_state/ecs/system/overworld/AnimationSystem.hpp"

#include "game_state/ecs/components/InputComponent.hpp"
#include "game_state/ecs/components/SpriteComponent.hpp"
#include "game_state/ecs/query/Query.hpp"

namespace Engine::ECS {

    void AnimationSystem::Update(SystemContext& context, float)
    {
        auto animated = context.CreateQuery<Including<SpriteComponent, InputComponent>>();

        // Walking diagonally: left/right wins over up/down, because x is checked first.
        // Standing still keeps the last frame.
        animated.ForEach([](Entity, SpriteComponent& sprite, InputComponent& input) {
            const sf::Vector2f walkDirection = input.walkDirection;

            if (walkDirection.x < 0.0f) {
                sprite.currentFrameNumber = sprite.standingLeftFrame;
            } else if (walkDirection.x > 0.0f) {
                sprite.currentFrameNumber = sprite.standingRightFrame;
            } else if (walkDirection.y < 0.0f) {
                sprite.currentFrameNumber = sprite.standingUpFrame;
            } else if (walkDirection.y > 0.0f) {
                sprite.currentFrameNumber = sprite.standingDownFrame;
            }
        });
    }

} // namespace Engine::ECS
