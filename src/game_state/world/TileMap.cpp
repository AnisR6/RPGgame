#include "game_state/world/TileMap.hpp"

#include <filesystem>
#include <fstream>
#include <iostream>
#include <nlohmann/json.hpp>


TileMap::TileMap(const std::string &mapFilePath, const std::string &tilesetFolderPath)
{
	std::ifstream mapFile(mapFilePath);
	if (!mapFile) {
		std::cout << "Could not open " << mapFilePath << "\n";
		return;
	}

	const nlohmann::json mapJson = nlohmann::json::parse(mapFile);

	// we extract the map data
	widthInTiles_ = mapJson["width"].get<int>();
	heightInTiles_ = mapJson["height"].get<int>();
	tileSizeInPixels_ = mapJson["tilewidth"].get<int>();
	// keep every tile layer (skip object layers)
	for (const nlohmann::json &layerJson : mapJson["layers"]) {
		if (layerJson["type"].get<std::string>() == "tilelayer") {
			tileLayers_.push_back(layerJson["data"].get<std::vector<int>>());
		}
	}

	// the tilesets
	const int tilesetCount = static_cast<int>(mapJson["tilesets"].size());
	tilesets_.resize(tilesetCount);

	// load every asset
	for (int tilesetIndex = 0; tilesetIndex < tilesetCount; ++tilesetIndex) {
		const nlohmann::json &tilesetJson = mapJson["tilesets"][tilesetIndex];
		Tileset &tileset = tilesets_[tilesetIndex];

		tileset.firstTileNumber = tilesetJson["firstgid"].get<int>();
		tileset.columnCount = tilesetJson["columns"].get<int>();

		// tiles marked "solid" in Tiled: remember their map numbers (firstgid + id)
		if (tilesetJson.contains("tiles")) {
			for (const nlohmann::json &tileJson : tilesetJson["tiles"]) {
				if (!tileJson.contains("properties")) {
					continue;
				}
				for (const nlohmann::json &propertyJson : tileJson["properties"]) {
					if (propertyJson["name"].get<std::string>() == "solid" && propertyJson["value"].get<bool>()) {
						solidTileNumbers_.insert(tileset.firstTileNumber + tileJson["id"].get<int>());
					}
				}
			}
		}  

		// to only keep the name of file instead of tiled generated path
		const std::string imagePathFromTiled = tilesetJson["image"].get<std::string>();
		const std::string imageFileName = std::filesystem::path(imagePathFromTiled).filename().string();
		const std::string imagePath = tilesetFolderPath + "/" + imageFileName;

		if (!tileset.picture.loadFromFile(imagePath)) {
			std::cout << "Could not load " << imagePath << "\n";
		}
	}
}

bool TileMap::isSolidTile(int tileX, int tileY) const

{
	// outside the map
	const bool isOutsideMap = tileX < 0 || tileY < 0 || tileX >= widthInTiles_ || tileY >= heightInTiles_;
	if (isOutsideMap) {
		return true;
	}


	// the top-most tile in this cell decides: grass on top of water is walkable, water alone is not
	for (int layerIndex = static_cast<int>(tileLayers_.size()) - 1; layerIndex >= 0; --layerIndex) {
		const int tileNumber = tileLayers_[layerIndex][tileY * widthInTiles_ + tileX];
		if (tileNumber != 0) {
			return solidTileNumbers_.count(tileNumber) > 0;
		}
	}

	return false; // no tiles
	
}

// which tileset does a map number belong to? The last one that starts at or before it.
int TileMap::findTilesetIndex(int tileNumber) const
{
	int foundIndex = 0;
	for (int tilesetIndex = 0; tilesetIndex < static_cast<int>(tilesets_.size()); ++tilesetIndex) {
		if (tilesets_[tilesetIndex].firstTileNumber <= tileNumber) {
			foundIndex = tilesetIndex;
		}
	}
	return foundIndex;
}

// cutting the PNG into squares and drawing it at the right place on the map
void TileMap::render(sf::RenderTarget &target) const
{
	for (const std::vector<int> &tileNumbers : tileLayers_) {
	
		for (int tileY = 0; tileY < heightInTiles_; ++tileY) {
		
			for (int tileX = 0; tileX < widthInTiles_; ++tileX) {

				const int tileNumber = tileNumbers[tileY * widthInTiles_ + tileX];

				if (tileNumber == 0) {
					continue; // empty cell, nothing to draw
				}

				// 1. which PNG

				const Tileset &tileset = tilesets_[findTilesetIndex(tileNumber)];

				// 2. which square inside that PNG

				const int squareIndexInPng = tileNumber - tileset.firstTileNumber;

				const int squareColumn = squareIndexInPng % tileset.columnCount;

				const int squareRow = squareIndexInPng / tileset.columnCount;

				// 3. we cut that square out of the PNG

				const sf::IntRect squareInPng({squareColumn * tileSizeInPixels_, squareRow * tileSizeInPixels_},
				                              {tileSizeInPixels_, tileSizeInPixels_});

				sf::Sprite tileSprite(tileset.picture, squareInPng);

				// 4. we draw it at its specific place on the map

				const float pixelX = static_cast<float>(tileX * tileSizeInPixels_);

				const float pixelY = static_cast<float>(tileY * tileSizeInPixels_);

				tileSprite.setPosition(sf::Vector2f(pixelX, pixelY));

				target.draw(tileSprite);
			}
		}
	}
}