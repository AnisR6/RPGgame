#include "presentation/rendering/AssetManager.hpp"

#include <filesystem>
#include <fstream>
#include <iostream>
#include <stdexcept>
#include <utility>

#include <nlohmann/json.hpp>

namespace {

    // -----------------------------------------------------------------------------
    // Tile Map Parsing
    // -----------------------------------------------------------------------------
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

} // namespace

// -----------------------------------------------------------------------------
// Tile Collision
// -----------------------------------------------------------------------------
bool TileMapData::IsSolidTileNumber(int tileNumber) const {
    return solidTileNumbers.count(tileNumber) > 0;
}

bool TileMapData::IsSolidTile(int tileX, int tileY) const {
    const bool isOutsideMap =
        tileX < 0 || tileY < 0 || tileX >= widthInTiles || tileY >= heightInTiles;

    if (isOutsideMap) {
        return true;
    }

    for (int layerIndex = static_cast<int>(layers.size()) - 1; layerIndex >= 0; --layerIndex) {
        if (!layers[layerIndex].isTileLayer) {
            continue;
        }

        const int tileNumber = layers[layerIndex].tileNumbers[tileY * widthInTiles + tileX];

        if (tileNumber != 0) {
            return IsSolidTileNumber(tileNumber);
        }
    }

    return false;
}

// -----------------------------------------------------------------------------
// Tileset Lookup
// -----------------------------------------------------------------------------
int TileMapData::FindTilesetIndex(int tileNumber) const {
    int foundIndex = 0;

    for (int tilesetIndex = 0; tilesetIndex < static_cast<int>(tilesets.size()); ++tilesetIndex) {
        if (tilesets[tilesetIndex].firstTileNumber > tileNumber) {
            break;
        }

        foundIndex = tilesetIndex;
    }

    return foundIndex;
}

// -----------------------------------------------------------------------------
// Texture Loading
// -----------------------------------------------------------------------------
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

// -----------------------------------------------------------------------------
// Font Loading
// -----------------------------------------------------------------------------
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

// -----------------------------------------------------------------------------
// Tile Map Loading
// -----------------------------------------------------------------------------
const TileMapData* AssetManager::LoadTileMap(const std::string& mapFilePath) {
    const auto existing = tileMaps.find(mapFilePath);

    if (existing != tileMaps.end()) {
        return &existing->second;
    }

    TileMapData tileMap = ParseTileMap(mapFilePath);

    auto [it, inserted] = tileMaps.emplace(mapFilePath, std::move(tileMap));

    return &it->second;
}