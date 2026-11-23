
#include "ecs/system/overworld/MapInitializationSystem.hpp"

#include "ecs/components/overworld/CollisionComponent.hpp"
#include "ecs/components/overworld/InputComponent.hpp"
#include "ecs/components/overworld/LayerComponent.hpp"
#include "ecs/components/overworld/MapObjectComponent.hpp"
#include "ecs/components/overworld/MapPointComponent.hpp"
#include "ecs/components/overworld/MovementComponent.hpp"
#include "ecs/components/overworld/PositionComponent.hpp"
#include "ecs/components/overworld/SpriteComponent.hpp"
#include "ecs/prefabs/overworld/Tile.hpp"
#include "presentation/rendering/AssetManager.hpp"

#include <vector>
#include <utility>

namespace {

    // -----------------------------------------------------------------------------
    // Tile Sprite Initialization
    // -----------------------------------------------------------------------------
    SpriteComponent CreateTileSpriteComponent(const TileMapData& tileMap, AssetManager& assets,
                                              const std::string& tilesetFolderPath,
                                              int tileNumber) {
        const Tileset& tileset = tileMap.tilesets[tileMap.FindTilesetIndex(tileNumber)];

        SpriteComponent spriteComponent;

        spriteComponent.spriteSheet =
            assets.LoadTexture(tilesetFolderPath + "/" + tileset.imageFile);

        spriteComponent.frameWidthInPixels = tileset.squareWidth;
        spriteComponent.frameHeightInPixels = tileset.squareHeight;
        spriteComponent.framesPerRow = tileset.columnCount;
        spriteComponent.currentFrameNumber = tileNumber - tileset.firstTileNumber;

        return spriteComponent;
    }

    // -----------------------------------------------------------------------------
    // Tile Layer Initialization
    // -----------------------------------------------------------------------------
    void CreateTileLayerEntities(Engine::ECS::SystemContext& context, const TileMapData& tileMap,
                                 AssetManager& assets, const std::string& tilesetFolderPath,
                                 const std::vector<int>& topmostLayerIndices, int layerIndex,
                                 const MapLayer& mapLayer) {
        const int mapWidthInTiles = tileMap.widthInTiles;
        const int tileSizeInPixels = tileMap.tileSizeInPixels;

        for (int tileY = 0; tileY < tileMap.heightInTiles; ++tileY) {
            for (int tileX = 0; tileX < mapWidthInTiles; ++tileX) {
                const int cellIndex = tileY * mapWidthInTiles + tileX;

                const int tileNumber = mapLayer.tileNumbers[cellIndex];

                if (tileNumber == 0) {
                    continue;
                }

                const sf::Vector2f tilePosition(static_cast<float>(tileX * tileSizeInPixels),
                                                static_cast<float>(tileY * tileSizeInPixels));

                SpriteComponent spriteComponent =
                    CreateTileSpriteComponent(tileMap, assets, tilesetFolderPath, tileNumber);

                CollisionComponent collisionComponent;
                collisionComponent.boxSizeInPixels = static_cast<float>(tileSizeInPixels);

                const bool isTopmostTile = topmostLayerIndices[cellIndex] == layerIndex;

                const bool isSolidTopmostTile =
                    isTopmostTile && tileMap.IsSolidTileNumber(tileNumber);

                Prefabs::TileBlueprint blueprint{PositionComponent{tilePosition},
                                                 std::move(spriteComponent),
                                                 LayerComponent{layerIndex}, collisionComponent};

                if (isSolidTopmostTile) {
                    Prefabs::CreateCollidableTilePrefab(context, std::move(blueprint));
                } else {
                    Prefabs::CreateTilePrefab(context, std::move(blueprint));
                }
            }
        }
    }

} // namespace

namespace Engine::ECS {

    // -----------------------------------------------------------------------------
    // Construction
    // -----------------------------------------------------------------------------
    MapInitializationSystem::MapInitializationSystem(const TileMapData& tileMap, AssetManager& assets,
                                                     std::string tilesetFolderPath,
                                                     sf::Vector2f playerStart,
                                                     std::string playerSpritePath)
        : tileMap(tileMap),
          assets(assets),
          tilesetFolderPath(std::move(tilesetFolderPath)),
          playerStart(playerStart),
          playerSpritePath(std::move(playerSpritePath)) {}

    // -----------------------------------------------------------------------------
    // System Update
    // -----------------------------------------------------------------------------
    void MapInitializationSystem::Update(SystemContext& context, float) {
        if (loaded) {
            return;
        }

        loaded = true;

        const int mapWidthInTiles = tileMap.widthInTiles;
        const int mapHeightInTiles = tileMap.heightInTiles;
        const int tileSizeInPixels = tileMap.tileSizeInPixels;

        const std::vector<MapLayer>& layers = tileMap.layers;

        const int mapCellCount = mapWidthInTiles * mapHeightInTiles;

        std::vector<int> topmostLayerIndices(mapCellCount, -1);

        // -------------------------------------------------------------------------
        // Determine Topmost Tiles
        // -------------------------------------------------------------------------
        for (int layerIndex = 0; layerIndex < static_cast<int>(layers.size()); ++layerIndex) {
            const MapLayer& layer = layers[layerIndex];

            if (!layer.isTileLayer) {
                continue;
            }

            for (int cellIndex = 0; cellIndex < mapCellCount; ++cellIndex) {
                if (layer.tileNumbers[cellIndex] != 0) {
                    topmostLayerIndices[cellIndex] = layerIndex;
                }
            }
        }

        // -------------------------------------------------------------------------
        // Create Map Entities
        // -------------------------------------------------------------------------
        const CollisionComponent playerCollision;
        const float halfBox = playerCollision.boxSizeInPixels / 2.0f;

        sf::Vector2f start = playerStart;

        for (int layerIndex = 0; layerIndex < static_cast<int>(layers.size()); ++layerIndex) {
            const MapLayer& layer = layers[layerIndex];

            if (layer.isTileLayer) {
                CreateTileLayerEntities(context, tileMap, assets, tilesetFolderPath,
                                        topmostLayerIndices, layerIndex, layer);

                continue;
            }

            for (const MapObject& object : layer.objects) {
                if (object.name == "PlayerSpawn") {
                    start = object.position - sf::Vector2f(halfBox, halfBox);

                    continue;
                }

                const Entity entity = context.CreateEntity();

                context.EmplaceComponent<PositionComponent>(entity,
                                                            PositionComponent{object.position});

                if (object.tileNumber == 0) {
                    context.EmplaceComponent<MapPointComponent>(
                        entity, MapPointComponent{object.name, object.type});

                    continue;
                }

                const Tileset& tileset =
                    tileMap.tilesets[tileMap.FindTilesetIndex(object.tileNumber)];

                SpriteComponent& spriteComponent =
                    context.EmplaceComponent<SpriteComponent>(entity);

                spriteComponent.spriteSheet =
                    assets.LoadTexture(tilesetFolderPath + "/" + tileset.imageFile);

                spriteComponent.frameWidthInPixels = tileset.squareWidth;

                spriteComponent.frameHeightInPixels = tileset.squareHeight;

                spriteComponent.framesPerRow = tileset.columnCount;

                spriteComponent.currentFrameNumber = object.tileNumber - tileset.firstTileNumber;

                context.EmplaceComponent<LayerComponent>(entity, LayerComponent{layerIndex});

                context.EmplaceComponent<MapObjectComponent>(
                    entity, MapObjectComponent{object.name.empty() ? tileset.name : object.name});
            }
        }

        // -------------------------------------------------------------------------
        // Player Initialization
        // -----------------------------------------------------------------------------
        const Entity player = context.CreateEntity();

        context.EmplaceComponent<PositionComponent>(player, PositionComponent{start});

        context.EmplaceComponent<CollisionComponent>(player, playerCollision);

        context.EmplaceComponent<MovementComponent>(player);
        context.EmplaceComponent<InputComponent>(player);

        SpriteComponent& playerSprite = context.EmplaceComponent<SpriteComponent>(player);

        playerSprite.spriteSheet = assets.LoadTexture(playerSpritePath);

        playerSprite.drawOffset =
            sf::Vector2f((playerCollision.boxSizeInPixels - playerSprite.frameWidthInPixels) / 2.0f,
                         playerCollision.boxSizeInPixels - playerSprite.frameHeightInPixels);
    }

} // namespace Engine::ECS
