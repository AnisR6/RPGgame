#pragma once

#include <SFML/Graphics/Texture.hpp>
#include <SFML/System/Vector2.hpp>

// -----------------------------------------------------------------------------
// Sprite Rendering Data
// -----------------------------------------------------------------------------
struct SpriteComponent {
        const sf::Texture* spriteSheet = nullptr;

        int frameWidthInPixels = 32;
        int frameHeightInPixels = 48;
        int framesPerRow = 4;
        int currentFrameNumber = 1;

        sf::Vector2f drawOffset;

        // -------------------------------------------------------------------------
        // Standing Frame Configuration
        // -------------------------------------------------------------------------
        int standingDownFrame = 1;
        int standingRightFrame = 5;
        int standingLeftFrame = 10;
        int standingUpFrame = 13;

        // -------------------------------------------------------------------------
        // Walking Animation State
        // -------------------------------------------------------------------------
        int facingStandingFrame = 1;
        int walkStep = 0;

        float secondsOnThisStep = 0.0f;
        float secondsPerStep = 0.15f;
};