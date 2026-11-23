#pragma once

#include "ecs/overworld/components/MapComponent.hpp"
#include "ecs/overworld/components/PositionComponent.hpp"
#include "ecs/shared/components/SpriteComponent.hpp"
#include "ecs/core/world/World.hpp"

#include <SFML/Graphics/Rect.hpp>
#include <SFML/Graphics/RenderTarget.hpp>

#include <vector>

class WorldRenderer {
    public:
        void Draw(sf::RenderTarget& target, Engine::ECS::World& world);

    private:
        struct WorldSprite {
                float bottom = 0.0f;
                const SpriteComponent* sprite = nullptr;
                const PositionComponent* position = nullptr;
        };

        void CollectVisibleSprites(const sf::RenderTarget& target, Engine::ECS::World& world,
                                   const MapComponent& map);

        static void DrawSprite(sf::RenderTarget& target, const SpriteComponent& sprite,
                               const PositionComponent& position);

        static sf::IntRect GetSpriteFrameRectangle(const SpriteComponent& sprite, int frameNumber);

        static bool IsSpriteOnScreen(const sf::FloatRect& cameraArea, const SpriteComponent& sprite,
                                     const PositionComponent& position);

        std::vector<std::vector<WorldSprite>> spritesByLayer;
};
