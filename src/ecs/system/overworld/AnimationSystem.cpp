#include "ecs/system/overworld/AnimationSystem.hpp"

#include "ecs/components/InputComponent.hpp"
#include "ecs/components/SpriteComponent.hpp"
#include "ecs/query/Query.hpp"

namespace Engine::ECS {

    void AnimationSystem::Update(SystemContext& context, float secondsSinceLastFrame)
    {
        auto animated = context.CreateQuery<Including<InputComponent, SpriteComponent>>();

        animated.ForEach([&](Entity, InputComponent& input, SpriteComponent& sprite) {
            const sf::Vector2f direction = input.walkDirection;

            // 1. which way do we look? (left/right wins over up/down)
            if (direction.x < 0.0f) {
                sprite.facingStandingFrame = sprite.standingLeftFrame;
            } else if (direction.x > 0.0f) {
                sprite.facingStandingFrame = sprite.standingRightFrame;
            } else if (direction.y < 0.0f) {
                sprite.facingStandingFrame = sprite.standingUpFrame;
            } else if (direction.y > 0.0f) {
                sprite.facingStandingFrame = sprite.standingDownFrame;
            }

            // 2. standing still: the standing frame
            if (direction.x == 0.0f && direction.y == 0.0f) {
                sprite.currentFrameNumber = sprite.facingStandingFrame;
                sprite.walkStep = 0;
                sprite.secondsOnThisStep = 0.0f;
                return;
            }

            // 3. walking: every 0.15 s the next of the 4 frames (0, 1, 2, 3, 0, 1...)
            sprite.secondsOnThisStep += secondsSinceLastFrame;
            if (sprite.secondsOnThisStep >= sprite.secondsPerStep) {
                sprite.secondsOnThisStep = 0.0f;
                sprite.walkStep = (sprite.walkStep + 1) % sprite.framesPerRow;
            }
            const int firstFrameOfRow = (sprite.facingStandingFrame / sprite.framesPerRow) * sprite.framesPerRow;
            sprite.currentFrameNumber = firstFrameOfRow + sprite.walkStep;
        });
    }

} // namespace Engine::ECS
