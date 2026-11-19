#include "game_state/ecs/system/overworld/MapInitializationSystem.hpp"

#include "game_state/ecs/components/CollisionComponent.hpp"
#include "game_state/ecs/components/InputComponent.hpp"
#include "game_state/ecs/components/LayerComponent.hpp"
#include "game_state/ecs/components/MapBoundsComponent.hpp"
#include "game_state/ecs/components/MovementComponent.hpp"
#include "game_state/ecs/components/PositionComponent.hpp"
#include "game_state/ecs/components/SpriteComponent.hpp"
#include "presentation/rendering/AssetManager.hpp"

namespace Engine::ECS {

    MapInitializationSystem::MapInitializationSystem(AssetManager& assets, std::string mapFilePath,
                                                      std::string tilesetFolderPath, sf::Vector2f playerStart,
                                                      std::string playerSpritePath)
        : assets(assets),
          mapFilePath(std::move(mapFilePath)),
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

        const LoadedMap map = assets.loadMap(mapFilePath, tilesetFolderPath);

        const Entity boundsEntity = context.CreateEntity();
        MapBoundsComponent& bounds = context.EmplaceComponent<MapBoundsComponent>(boundsEntity);
        bounds.widthInTiles = map.widthInTiles;
        bounds.heightInTiles = map.heightInTiles;
        bounds.tileSizeInPixels = map.tileSizeInPixels;

        for (const TopTile& tile : map.tiles) {
            const Entity tileEntity = context.CreateEntity();

            PositionComponent& position = context.EmplaceComponent<PositionComponent>(tileEntity);
            position.position = sf::Vector2f(static_cast<float>(tile.tileX * map.tileSizeInPixels),
                                              static_cast<float>(tile.tileY * map.tileSizeInPixels));

            SpriteComponent& sprite = context.EmplaceComponent<SpriteComponent>(tileEntity);
            sprite.spriteSheet = tile.texture;
            sprite.frameWidthInPixels = tile.frameSizeInPixels;
            sprite.frameHeightInPixels = tile.frameSizeInPixels;
            sprite.framesPerRow = tile.framesPerRow;
            sprite.currentFrameNumber = tile.frameNumber;

            LayerComponent& layer = context.EmplaceComponent<LayerComponent>(tileEntity);
            layer.zIndex = tile.zIndex;

            if (tile.solid) {
                CollisionComponent& collision = context.EmplaceComponent<CollisionComponent>(tileEntity);
                collision.boxSizeInPixels = static_cast<float>(map.tileSizeInPixels);
                collision.solid = true;
            }
        }

        const Entity player = context.CreateEntity();

        PositionComponent& playerPosition = context.EmplaceComponent<PositionComponent>(player);
        playerPosition.position = playerStart;

        context.EmplaceComponent<CollisionComponent>(player);
        context.EmplaceComponent<MovementComponent>(player);
        context.EmplaceComponent<InputComponent>(player);

        LayerComponent& playerLayer = context.EmplaceComponent<LayerComponent>(player);
        playerLayer.zIndex = map.layerCount;

        SpriteComponent& playerSprite = context.EmplaceComponent<SpriteComponent>(player);
        playerSprite.spriteSheet = &assets.loadTexture(playerSpritePath);
    }

} // namespace Engine::ECS
