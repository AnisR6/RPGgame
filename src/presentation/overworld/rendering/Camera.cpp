#include "presentation/overworld/rendering/Camera.hpp"

#include "ecs/overworld/components/InputComponent.hpp"
#include "ecs/overworld/components/PositionComponent.hpp"
#include "ecs/shared/components/SpriteComponent.hpp"
#include "ecs/overworld/systems/MapLookup.hpp"

#include <algorithm>

namespace {
    float ClampToMap(float center, float viewLength, float mapLength) {
        if (mapLength <= viewLength) {
            return mapLength / 2.0f;
        }

        return std::clamp(center, viewLength / 2.0f, mapLength - viewLength / 2.0f);
    }
}

namespace Camera {
    sf::View FollowPlayer(Engine::ECS::World& world, sf::Vector2f viewSize) {
        sf::Vector2f center;

        const MapComponent* map = MapLookup::FindMap(world);

        if (map == nullptr) {
            return sf::View(center, viewSize);
        }

        auto controlled = world.CreateQuery<
            Engine::ECS::Including<InputComponent, PositionComponent, SpriteComponent>>();

        controlled.ForEach([&](Engine::ECS::Entity, InputComponent&, PositionComponent& position,
                               SpriteComponent& sprite) {
            center =
                position.position + sprite.drawOffset +
                sf::Vector2f(sprite.frameWidthInPixels / 2.0f, sprite.frameHeightInPixels / 2.0f);
        });

        const float mapWidth = static_cast<float>(map->widthInTiles * map->tileSizeInPixels);
        const float mapHeight = static_cast<float>(map->heightInTiles * map->tileSizeInPixels);

        center.x = ClampToMap(center.x, viewSize.x, mapWidth);
        center.y = ClampToMap(center.y, viewSize.y, mapHeight);

        return sf::View(center, viewSize);
    }
}
