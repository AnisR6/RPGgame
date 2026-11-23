#include "ecs/system/overworld/MapInitializationSystem.hpp"

#include "ecs/components/overworld/CollisionComponent.hpp"
#include "ecs/components/overworld/InputComponent.hpp"
#include "ecs/components/overworld/LayerComponent.hpp"
#include "ecs/components/overworld/MapObjectComponent.hpp"
#include "ecs/components/overworld/MapPointComponent.hpp"
#include "ecs/components/overworld/MovementComponent.hpp"
#include "ecs/components/overworld/PositionComponent.hpp"
#include "ecs/components/overworld/SpriteComponent.hpp"
#include "persistence/Tilemap.hpp"
#include "presentation/rendering/AssetManager.hpp"

namespace Engine::ECS {

    MapInitializationSystem::MapInitializationSystem(const TileMap& tileMap, AssetManager& assets,
                                                     std::string tilesetFolderPath, sf::Vector2f playerStart,
                                                     std::string playerSpritePath)
        : tileMap(tileMap),
          assets(assets),
          tilesetFolderPath(std::move(tilesetFolderPath)),
          playerStart(playerStart),
          playerSpritePath(std::move(playerSpritePath))
    {
    }

    void MapInitializationSystem::Update(SystemContext& context, float)
    {
        if (loaded) {
            return;
        }
        loaded = true;

        const CollisionComponent playerCollision;
        const float halfBox = playerCollision.boxSizeInPixels / 2.0f;
        sf::Vector2f start = playerStart;

        // every object of the map becomes an entity
        const std::vector<MapLayer>& layers = tileMap.getLayers();
        for (int layerIndex = 0; layerIndex < static_cast<int>(layers.size()); ++layerIndex) {
            for (const MapObject& object : layers[layerIndex].objects) {
                if (object.name == "PlayerSpawn") {
                    start = object.position - sf::Vector2f(halfBox, halfBox); // his feet on the point
                    continue;
                }

                const Entity entity = context.CreateEntity();
                context.EmplaceComponent<PositionComponent>(entity, PositionComponent{object.position});

                if (object.tileNumber == 0) {
                    // a point: NPC, save point, enemy...
                    context.EmplaceComponent<MapPointComponent>(entity, MapPointComponent{object.name, object.type});
                    continue;
                }

                // a picture: tree, house, sheep, foam...
                const Tileset& tileset = tileMap.getTilesets()[tileMap.findTilesetIndex(object.tileNumber)];
                SpriteComponent& sprite = context.EmplaceComponent<SpriteComponent>(entity);
                sprite.spriteSheet = assets.loadTexture(tilesetFolderPath + "/" + tileset.imageFile);
                sprite.frameWidthInPixels = tileset.squareWidth;
                sprite.frameHeightInPixels = tileset.squareHeight;
                sprite.framesPerRow = tileset.columnCount;
                sprite.currentFrameNumber = object.tileNumber - tileset.firstTileNumber; // which square of the PNG

                context.EmplaceComponent<LayerComponent>(entity, LayerComponent{layerIndex}); // drawn in its Tiled layer
                context.EmplaceComponent<MapObjectComponent>(entity, MapObjectComponent{object.name.empty() ? tileset.name : object.name});
            }
        }

        // the player. No LayerComponent: he is drawn with the trees and houses, sorted by y.
        const Entity player = context.CreateEntity();
        context.EmplaceComponent<PositionComponent>(player, PositionComponent{start});
        context.EmplaceComponent<CollisionComponent>(player, playerCollision);
        context.EmplaceComponent<MovementComponent>(player);
        context.EmplaceComponent<InputComponent>(player);

        SpriteComponent& playerSprite = context.EmplaceComponent<SpriteComponent>(player);
        playerSprite.spriteSheet = assets.loadTexture(playerSpritePath);
        // his position is the collision box at his feet: the picture is centered on it and goes up
        playerSprite.drawOffset = sf::Vector2f((playerCollision.boxSizeInPixels - playerSprite.frameWidthInPixels) / 2.0f,
                                               playerCollision.boxSizeInPixels - playerSprite.frameHeightInPixels);
    }

} // namespace Engine::ECS
