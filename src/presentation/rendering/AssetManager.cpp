#include "presentation/rendering/AssetManager.hpp"

#include <filesystem>
#include <fstream>
#include <iostream>
#include <unordered_set>

#include <nlohmann/json.hpp>

const sf::Texture &AssetManager::loadTexture(const std::string &path)
{
	const auto existing = textures.find(path);
	if (existing != textures.end()) {
		return existing->second;
	}

	sf::Texture texture;
	if (!texture.loadFromFile(path)) {
		std::cout << "Could not load " << path << "\n";
	}

	const auto inserted = textures.emplace(path, std::move(texture));
	return inserted.first->second;
}

LoadedMap AssetManager::loadMap(const std::string &mapFilePath, const std::string &tilesetFolderPath)
{
	LoadedMap loadedMap;

	std::ifstream mapFile(mapFilePath);
	if (!mapFile) {
		std::cout << "Could not open " << mapFilePath << "\n";
		return loadedMap;
	}

	const nlohmann::json mapJson = nlohmann::json::parse(mapFile);

	loadedMap.widthInTiles = mapJson["width"].get<int>();
	loadedMap.heightInTiles = mapJson["height"].get<int>();
	loadedMap.tileSizeInPixels = mapJson["tilewidth"].get<int>();

	// Every tile layer, bottom first. Used only to pick the topmost tile, then discarded.
	std::vector<std::vector<int>> tileLayers;
	for (const nlohmann::json &layerJson : mapJson["layers"]) {
		if (layerJson["type"].get<std::string>() == "tilelayer") {
			tileLayers.push_back(layerJson["data"].get<std::vector<int>>());
		}
	}
	loadedMap.layerCount = static_cast<int>(tileLayers.size());

	struct TilesetRecord {
		int firstTileNumber = 0;
		int columnCount = 0;
		const sf::Texture *texture = nullptr;
	};

	const int tilesetCount = static_cast<int>(mapJson["tilesets"].size());
	std::vector<TilesetRecord> tilesets(tilesetCount);
	std::unordered_set<int> solidTileNumbers;

	for (int tilesetIndex = 0; tilesetIndex < tilesetCount; ++tilesetIndex) {
		const nlohmann::json &tilesetJson = mapJson["tilesets"][tilesetIndex];
		TilesetRecord &tileset = tilesets[tilesetIndex];

		tileset.firstTileNumber = tilesetJson["firstgid"].get<int>();
		tileset.columnCount = tilesetJson["columns"].get<int>();

		if (tilesetJson.contains("tiles")) {
			for (const nlohmann::json &tileJson : tilesetJson["tiles"]) {
				if (!tileJson.contains("properties")) {
					continue;
				}
				for (const nlohmann::json &propertyJson : tileJson["properties"]) {
					if (propertyJson["name"].get<std::string>() == "solid" && propertyJson["value"].get<bool>()) {
						solidTileNumbers.insert(tileset.firstTileNumber + tileJson["id"].get<int>());
					}
				}
			}
		}

		const std::string imagePathFromTiled = tilesetJson["image"].get<std::string>();
		const std::string imageFileName = std::filesystem::path(imagePathFromTiled).filename().string();
		const std::string imagePath = tilesetFolderPath + "/" + imageFileName;
		tileset.texture = &loadTexture(imagePath);
	}

	// The last tileset that starts at or before this map number owns it.
	const auto findTilesetIndex = [&tilesets](int tileNumber) {
		int foundIndex = 0;
		for (int tilesetIndex = 0; tilesetIndex < static_cast<int>(tilesets.size()); ++tilesetIndex) {
			if (tilesets[tilesetIndex].firstTileNumber <= tileNumber) {
				foundIndex = tilesetIndex;
			}
		}
		return foundIndex;
	};

	for (int tileY = 0; tileY < loadedMap.heightInTiles; ++tileY) {
		for (int tileX = 0; tileX < loadedMap.widthInTiles; ++tileX) {
			for (int layerIndex = static_cast<int>(tileLayers.size()) - 1; layerIndex >= 0; --layerIndex) {
				const std::vector<int> &tileNumbers = tileLayers[layerIndex];
				const int tileNumber = tileNumbers[tileY * loadedMap.widthInTiles + tileX];
				if (tileNumber == 0) {
					continue;
				}

				const TilesetRecord &tileset = tilesets[findTilesetIndex(tileNumber)];

				TopTile tile;
				tile.tileX = tileX;
				tile.tileY = tileY;
				tile.zIndex = layerIndex;
				tile.frameNumber = tileNumber - tileset.firstTileNumber;
				tile.framesPerRow = tileset.columnCount;
				tile.frameSizeInPixels = loadedMap.tileSizeInPixels;
				tile.solid = solidTileNumbers.count(tileNumber) > 0;
				tile.texture = tileset.texture;
				loadedMap.tiles.push_back(tile);
				break;
			}
		}
	}

	return loadedMap;
}
