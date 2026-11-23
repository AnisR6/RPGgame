#include "ecs/system/overworld/AnimationSystem.hpp"

#include "ecs/components/InputComponent.hpp"
#include "ecs/components/SpriteComponent.hpp"
#include "ecs/query/Query.hpp"

namespace {

    // -----------------------------------------------------------------------------
    // Facing Direction
    // -----------------------------------------------------------------------------
    void UpdateFacingStandingFrame(const sf::Vector2f& movementDirection,
                                   SpriteComponent& spriteComponent) {
        if (movementDirection.x < 0.0f) {
            spriteComponent.facingStandingFrame = spriteComponent.standingLeftFrame;
        } else if (movementDirection.x > 0.0f) {
            spriteComponent.facingStandingFrame = spriteComponent.standingRightFrame;
        } else if (movementDirection.y < 0.0f) {
            spriteComponent.facingStandingFrame = spriteComponent.standingUpFrame;
        } else if (movementDirection.y > 0.0f) {
            spriteComponent.facingStandingFrame = spriteComponent.standingDownFrame;
        }
    }

    // -----------------------------------------------------------------------------
    // Standing Animation
    // -----------------------------------------------------------------------------
    bool IsMovementDirectionZero(const sf::Vector2f& movementDirection) {
        return movementDirection.x == 0.0f && movementDirection.y == 0.0f;
    }

    void UpdateStandingAnimationFrame(SpriteComponent& spriteComponent) {
        spriteComponent.currentFrameNumber = spriteComponent.facingStandingFrame;

        spriteComponent.walkStep = 0;
        spriteComponent.secondsOnThisStep = 0.0f;
    }

    // -----------------------------------------------------------------------------
    // Walking Animation
    // -----------------------------------------------------------------------------
    void AdvanceWalkingAnimationFrame(SpriteComponent& spriteComponent, float deltaTime) {
        spriteComponent.secondsOnThisStep += deltaTime;

        if (spriteComponent.secondsOnThisStep >= spriteComponent.secondsPerStep) {
            spriteComponent.secondsOnThisStep = 0.0f;

            spriteComponent.walkStep =
                (spriteComponent.walkStep + 1) % spriteComponent.framesPerRow;
        }

        const int firstFrameOfRow =
            (spriteComponent.facingStandingFrame / spriteComponent.framesPerRow) *
            spriteComponent.framesPerRow;

        spriteComponent.currentFrameNumber = firstFrameOfRow + spriteComponent.walkStep;
    }

    // -----------------------------------------------------------------------------
    // Sprite Animation
    // -----------------------------------------------------------------------------
    void UpdateSpriteAnimationForInput(const sf::Vector2f& movementDirection,
                                       SpriteComponent& spriteComponent, float deltaTime) {
        UpdateFacingStandingFrame(movementDirection, spriteComponent);

        if (IsMovementDirectionZero(movementDirection)) {
            UpdateStandingAnimationFrame(spriteComponent);
            return;
        }

        AdvanceWalkingAnimationFrame(spriteComponent, deltaTime);
    }

} // namespace

namespace Engine::ECS {

    // -----------------------------------------------------------------------------
    // System Update
    // -----------------------------------------------------------------------------
    void AnimationSystem::Update(SystemContext& context, float deltaTime) {
        auto animatedEntities = context.CreateQuery<Including<InputComponent, SpriteComponent>>();

        animatedEntities.ForEach([&](Entity, InputComponent& inputComponent,
                                     SpriteComponent& spriteComponent) {
            UpdateSpriteAnimationForInput(inputComponent.walkDirection, spriteComponent, deltaTime);
        });
    }

} // namespace Engine::ECS