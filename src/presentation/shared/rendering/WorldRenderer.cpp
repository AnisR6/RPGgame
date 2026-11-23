#include "presentation/shared/rendering/WorldRenderer.hpp"

#include "ecs/overworld/components/LayerComponent.hpp"
#include "ecs/overworld/systems/MapLookup.hpp"

#include <SFML/Graphics/Sprite.hpp>

#include <algorithm>

void WorldRenderer::Draw(sf::RenderTarget& target, Engine::ECS::World& world) {
    const MapComponent* map = MapLookup::FindMap(world);

    if (map == nullptr || map->sortedLayerIndex < 0 || map->sortedLayerIndex >= map->layerCount) {
        return;
    }

    CollectVisibleSprites(target, world, *map);

    std::vector<WorldSprite>& sortedLayer = spritesByLayer[map->sortedLayerIndex];

    std::stable_sort(sortedLayer.begin(), sortedLayer.end(),
                     [](const WorldSprite& first, const WorldSprite& second) {
                         return first.bottom < second.bottom;
                     });

    for (const std::vector<WorldSprite>& layer : spritesByLayer) {
        for (const WorldSprite& worldSprite : layer) {
            DrawSprite(target, *worldSprite.sprite, *worldSprite.position);
        }
    }
}

void WorldRenderer::CollectVisibleSprites(const sf::RenderTarget& target, Engine::ECS::World& world,
                                          const MapComponent& map) {
    spritesByLayer.resize(map.layerCount);

    for (std::vector<WorldSprite>& layer : spritesByLayer) {
        layer.clear();
    }

    const sf::View& camera = target.getView();
    const sf::FloatRect cameraArea(camera.getCenter() - camera.getSize() / 2.0f, camera.getSize());

    auto drawable = world.CreateQuery<Engine::ECS::Including<SpriteComponent, PositionComponent>>();

    drawable.ForEach([&](Engine::ECS::Entity entity, SpriteComponent& sprite,
                         PositionComponent& position) {
        if (!IsSpriteOnScreen(cameraArea, sprite, position)) {
            return;
        }

        int layerIndex = map.sortedLayerIndex;

        if (world.HasComponent<LayerComponent>(entity)) {
            const int componentLayerIndex = world.GetComponent<LayerComponent>(entity).layerIndex;

            if (componentLayerIndex >= 0 && componentLayerIndex < map.layerCount) {
                layerIndex = componentLayerIndex;
            }
        }

        const float bottom = position.position.y + sprite.drawOffset.y + sprite.frameHeightInPixels;

        spritesByLayer[layerIndex].push_back({bottom, &sprite, &position});
    });
}

void WorldRenderer::DrawSprite(sf::RenderTarget& target, const SpriteComponent& sprite,
                               const PositionComponent& position) {
    if (sprite.spriteSheet == nullptr) {
        return;
    }

    if (sprite.framesPerRow <= 0 || sprite.frameWidthInPixels <= 0 ||
        sprite.frameHeightInPixels <= 0) {
        return;
    }

    sf::Sprite frameSprite(*sprite.spriteSheet,
                           GetSpriteFrameRectangle(sprite, sprite.currentFrameNumber));

    frameSprite.setPosition(position.position + sprite.drawOffset);
    frameSprite.setColor(sprite.tint);

    target.draw(frameSprite);
}

sf::IntRect WorldRenderer::GetSpriteFrameRectangle(const SpriteComponent& sprite, int frameNumber) {
    const int safeFramesPerRow = std::max(1, sprite.framesPerRow);

    const int column = frameNumber % safeFramesPerRow;
    const int row = frameNumber / safeFramesPerRow;

    const sf::Vector2i topLeftCorner(column * sprite.frameWidthInPixels,
                                     row * sprite.frameHeightInPixels);

    const sf::Vector2i frameSize(sprite.frameWidthInPixels, sprite.frameHeightInPixels);

    return sf::IntRect(topLeftCorner, frameSize);
}

bool WorldRenderer::IsSpriteOnScreen(const sf::FloatRect& cameraArea, const SpriteComponent& sprite,
                                     const PositionComponent& position) {
    const sf::FloatRect pictureArea(position.position + sprite.drawOffset,
                                    sf::Vector2f(static_cast<float>(sprite.frameWidthInPixels),
                                                 static_cast<float>(sprite.frameHeightInPixels)));

    return cameraArea.findIntersection(pictureArea).has_value();
}
