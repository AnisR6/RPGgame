#include "presentation/shared/rendering/AssetManager.hpp"

#include "ecs/overworld/components/CollisionComponent.hpp"
#include "ecs/overworld/components/LayerComponent.hpp"
#include "ecs/overworld/components/MapComponent.hpp"
#include "ecs/overworld/components/MapObjectComponent.hpp"
#include "ecs/overworld/components/MapPointComponent.hpp"
#include "ecs/overworld/components/PositionComponent.hpp"
#include "ecs/shared/components/SpriteComponent.hpp"
#include "ecs/overworld/prefabs/Tile.hpp"
#include "ecs/core/world/World.hpp"

#include <SFML/System/Vector2.hpp>

#include <filesystem>
#include <fstream>
#include <iostream>
#include <set>
#include <stdexcept>
#include <utility>
#include <vector>

#include <nlohmann/json.hpp>

namespace {
    constexpr const char* CollisionLayerName = "Collision";

    struct Tileset {
            int firstTileNumber = 0;
            int columnCount = 0;
            int squareWidth = 32;
            int squareHeight = 32;
            std::string imageFile;
            std::string name;
    };

    struct MapObject {
            int tileNumber = 0;
            std::string name;
            std::string type;
            sf::Vector2f position;
    };

    struct MapLayer {
            std::string name;
            bool isTileLayer = true;
            bool sortWithPlayer = false;
            std::vector<int> tileNumbers;
            std::vector<MapObject> objects;
    };

    struct TileMapData {
            int widthInTiles = 0;
            int heightInTiles = 0;
            int tileSizeInPixels = 32;

            std::vector<MapLayer> layers;
            std::set<int> solidTileNumbers;
            std::vector<Tileset> tilesets;

            bool IsSolidTileNumber(int tileNumber) const {
                return solidTileNumbers.count(tileNumber) > 0;
            }

            bool IsSolidTile(int tileX, int tileY) const {
                const bool isOutsideMap =
                    tileX < 0 || tileY < 0 || tileX >= widthInTiles || tileY >= heightInTiles;

                if (isOutsideMap) {
                    return true;
                }

                for (int layerIndex = static_cast<int>(layers.size()) - 1; layerIndex >= 0;
                     --layerIndex) {
                    if (!layers[layerIndex].isTileLayer) {
                        continue;
                    }

                    const int tileNumber =
                        layers[layerIndex].tileNumbers[tileY * widthInTiles + tileX];

                    if (tileNumber != 0) {
                        return IsSolidTileNumber(tileNumber);
                    }
                }

                return false;
            }

            int FindTilesetIndex(int tileNumber) const {
                int foundIndex = 0;

                for (int tilesetIndex = 0; tilesetIndex < static_cast<int>(tilesets.size());
                     ++tilesetIndex) {
                    if (tilesets[tilesetIndex].firstTileNumber > tileNumber) {
                        break;
                    }

                    foundIndex = tilesetIndex;
                }

                return foundIndex;
            }
    };

    TileMapData ParseTileMap(const std::string& mapFilePath) {
        TileMapData tileMap;

        std::ifstream mapFile(mapFilePath);

        if (!mapFile) {
            std::cout << "Could not open " << mapFilePath << "\n";
            return tileMap;
        }

        const nlohmann::json mapJson = nlohmann::json::parse(mapFile);

        tileMap.widthInTiles = mapJson["width"].get<int>();
        tileMap.heightInTiles = mapJson["height"].get<int>();
        tileMap.tileSizeInPixels = mapJson["tilewidth"].get<int>();

        for (const nlohmann::json& tilesetJson : mapJson["tilesets"]) {
            Tileset tileset;

            tileset.firstTileNumber = tilesetJson["firstgid"].get<int>();
            tileset.columnCount = tilesetJson["columns"].get<int>();
            tileset.squareWidth = tilesetJson["tilewidth"].get<int>();
            tileset.squareHeight = tilesetJson["tileheight"].get<int>();
            tileset.name = tilesetJson["name"].get<std::string>();
            tileset.imageFile =
                std::filesystem::path(tilesetJson["image"].get<std::string>()).filename().string();

            if (tilesetJson.contains("tiles")) {
                for (const nlohmann::json& tileJson : tilesetJson["tiles"]) {
                    for (const nlohmann::json& propertyJson :
                         tileJson.value("properties", nlohmann::json::array())) {
                        if (propertyJson["name"] == "solid" && propertyJson["value"] == true) {
                            tileMap.solidTileNumbers.insert(tileset.firstTileNumber +
                                                            tileJson["id"].get<int>());
                        }
                    }
                }
            }

            tileMap.tilesets.push_back(tileset);
        }

        for (const nlohmann::json& layerJson : mapJson["layers"]) {
            MapLayer layer;
            layer.name = layerJson["name"].get<std::string>();

            for (const nlohmann::json& propertyJson :
                 layerJson.value("properties", nlohmann::json::array())) {
                if (propertyJson["name"] == "ysort" && propertyJson["value"] == true) {
                    layer.sortWithPlayer = true;
                }
            }

            if (layerJson["type"] == "tilelayer") {
                layer.tileNumbers = layerJson["data"].get<std::vector<int>>();
            } else if (layerJson["type"] == "objectgroup") {
                layer.isTileLayer = false;

                for (const nlohmann::json& objectJson : layerJson["objects"]) {
                    MapObject object;

                    object.name = objectJson["name"].get<std::string>();
                    object.type = objectJson.value("type", "");

                    object.position =
                        sf::Vector2f(objectJson["x"].get<float>(), objectJson["y"].get<float>());

                    if (objectJson.contains("gid")) {
                        object.tileNumber = objectJson["gid"].get<int>();

                        object.position.y -= objectJson["height"].get<float>();
                    }

                    layer.objects.push_back(object);
                }
            } else {
                continue;
            }

            tileMap.layers.push_back(layer);
        }

        return tileMap;
    }

    int FindSortedLayerIndex(const TileMapData& tileMap) {
        for (int layerIndex = 0; layerIndex < static_cast<int>(tileMap.layers.size());
             ++layerIndex) {
            if (tileMap.layers[layerIndex].sortWithPlayer) {
                return layerIndex;
            }
        }

        return static_cast<int>(tileMap.layers.size()) - 1;
    }

    int ResolveEntityLayerIndex(const MapLayer& layer, int layerIndex, int sortedLayerIndex) {
        return layer.sortWithPlayer ? sortedLayerIndex : layerIndex;
    }

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

    void CreateTileLayerEntities(Engine::ECS::World& world, const TileMapData& tileMap,
                                 AssetManager& assets, const std::string& tilesetFolderPath,
                                 const std::vector<int>& topmostLayerIndices, int layerIndex,
                                 int entityLayerIndex, const MapLayer& mapLayer) {
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

                Prefabs::TileBlueprint blueprint{
                    PositionComponent{tilePosition}, std::move(spriteComponent),
                    LayerComponent{entityLayerIndex}, collisionComponent};

                if (isSolidTopmostTile) {
                    Prefabs::CreateCollidableTilePrefab(world, std::move(blueprint));
                } else {
                    Prefabs::CreateTilePrefab(world, std::move(blueprint));
                }
            }
        }
    }

    void CreateObjectLayerEntities(Engine::ECS::World& world, const TileMapData& tileMap,
                                   AssetManager& assets, const std::string& tilesetFolderPath,
                                   int entityLayerIndex, const MapLayer& mapLayer) {
        for (const MapObject& object : mapLayer.objects) {
            const Engine::ECS::Entity entity = world.CreateEntity();

            world.EmplaceComponent<PositionComponent>(entity, PositionComponent{object.position});

            if (object.tileNumber == 0) {
                world.EmplaceComponent<MapPointComponent>(
                    entity, MapPointComponent{object.name, object.type});

                continue;
            }

            const Tileset& tileset = tileMap.tilesets[tileMap.FindTilesetIndex(object.tileNumber)];

            world.EmplaceComponent<SpriteComponent>(
                entity,
                CreateTileSpriteComponent(tileMap, assets, tilesetFolderPath, object.tileNumber));

            world.EmplaceComponent<LayerComponent>(entity, LayerComponent{entityLayerIndex});

            world.EmplaceComponent<MapObjectComponent>(
                entity, MapObjectComponent{object.name.empty() ? tileset.name : object.name});
        }
    }

    Engine::ECS::Entity CreateMapEntity(Engine::ECS::World& world, const TileMapData& tileMap,
                                        int sortedLayerIndex) {
        MapComponent mapComponent;
        mapComponent.widthInTiles = tileMap.widthInTiles;
        mapComponent.heightInTiles = tileMap.heightInTiles;
        mapComponent.tileSizeInPixels = tileMap.tileSizeInPixels;
        mapComponent.layerCount = static_cast<int>(tileMap.layers.size());
        mapComponent.sortedLayerIndex = sortedLayerIndex;
        mapComponent.solidCells.resize(
            static_cast<std::size_t>(tileMap.widthInTiles * tileMap.heightInTiles));

        for (int tileY = 0; tileY < tileMap.heightInTiles; ++tileY) {
            for (int tileX = 0; tileX < tileMap.widthInTiles; ++tileX) {
                mapComponent.solidCells[tileY * tileMap.widthInTiles + tileX] =
                    tileMap.IsSolidTile(tileX, tileY) ? 1 : 0;
            }
        }

        const Engine::ECS::Entity mapEntity = world.CreateEntity();
        world.EmplaceComponent<MapComponent>(mapEntity, std::move(mapComponent));
        return mapEntity;
    }
}

const sf::Texture* AssetManager::LoadTexture(const std::string& path) {
    const auto existing = textures.find(path);

    if (existing != textures.end()) {
        return &existing->second;
    }

    if (!std::filesystem::exists(path)) {
        std::cout << "Could not find " << path << "\n";
        return nullptr;
    }

    sf::Texture& texture = textures[path];

    if (!texture.loadFromFile(path)) {
        std::cout << "Could not load " << path << "\n";
    }

    return &texture;
}

void AssetManager::LoadFont(FontId id, const std::string& path) {
    auto font = std::make_unique<sf::Font>();

    if (!font->openFromFile(path)) {
        throw std::runtime_error("AssetManager: failed to load font: " + path);
    }

    fonts.insert_or_assign(id, std::move(font));
}

const sf::Font& AssetManager::GetFont(FontId id) const {
    const auto it = fonts.find(id);

    if (it == fonts.end()) {
        throw std::out_of_range("AssetManager: requested font has not been loaded");
    }

    return *it->second;
}

bool AssetManager::HasFont(FontId id) const noexcept {
    return fonts.find(id) != fonts.end();
}

Engine::ECS::Entity AssetManager::LoadTileMap(const std::string& mapFilePath,
                                              const std::string& tilesetFolderPath,
                                              Engine::ECS::World& world) {
    const TileMapData tileMap = ParseTileMap(mapFilePath);

    const int sortedLayerIndex = FindSortedLayerIndex(tileMap);

    const int mapCellCount = tileMap.widthInTiles * tileMap.heightInTiles;

    std::vector<int> topmostLayerIndices(mapCellCount, -1);

    for (int layerIndex = 0; layerIndex < static_cast<int>(tileMap.layers.size()); ++layerIndex) {
        const MapLayer& layer = tileMap.layers[layerIndex];

        if (!layer.isTileLayer) {
            continue;
        }

        for (int cellIndex = 0; cellIndex < mapCellCount; ++cellIndex) {
            if (layer.tileNumbers[cellIndex] != 0) {
                topmostLayerIndices[cellIndex] = layerIndex;
            }
        }
    }

    for (int layerIndex = 0; layerIndex < static_cast<int>(tileMap.layers.size()); ++layerIndex) {
        const MapLayer& layer = tileMap.layers[layerIndex];

        const int entityLayerIndex = ResolveEntityLayerIndex(layer, layerIndex, sortedLayerIndex);

        if (layer.name == CollisionLayerName) {
            continue;
        }

        if (layer.isTileLayer) {
            CreateTileLayerEntities(world, tileMap, *this, tilesetFolderPath, topmostLayerIndices,
                                    layerIndex, entityLayerIndex, layer);
        } else {
            CreateObjectLayerEntities(world, tileMap, *this, tilesetFolderPath, entityLayerIndex,
                                      layer);
        }
    }

    return CreateMapEntity(world, tileMap, sortedLayerIndex);
}
