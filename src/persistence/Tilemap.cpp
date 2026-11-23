#include "persistence/Tilemap.hpp"

#include <filesystem>
#include <fstream>
#include <iostream>
#include <nlohmann/json.hpp>


TileMap::TileMap(const std::string& mapFilePath)
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

	// keep every tile layer 
	for (const nlohmann::json& tilesetJson : mapJson["tilesets"]) {
		Tileset tileset;
		tileset.firstTileNumber = tilesetJson["firstgid"].get<int>();
		tileset.columnCount = tilesetJson["columns"].get<int>();
		tileset.squareWidth = tilesetJson["tilewidth"].get<int>();
		tileset.squareHeight = tilesetJson["tileheight"].get<int>();
		tileset.name = tilesetJson["name"].get<std::string>();
		tileset.imageFile = std::filesystem::path(tilesetJson["image"].get<std::string>()).filename().string();


		if (tilesetJson.contains("tiles")) {
			for (const nlohmann::json& tileJson : tilesetJson["tiles"]) {
				for (const nlohmann::json& propertyJson : tileJson.value("properties", nlohmann::json::array())) {
					if (propertyJson["name"] == "solid" && propertyJson["value"] == true) {
						solidTileNumbers_.insert(tileset.firstTileNumber + tileJson["id"].get<int>());
					}
				}
			}
		}
		tilesets_.push_back(tileset);
	}

	// layers
	for (const nlohmann::json& layerJson : mapJson["layers"]) {
		MapLayer layer;
		layer.name = layerJson["name"].get<std::string>();

		for (const nlohmann::json& propertyJson : layerJson.value("properties", nlohmann::json::array())) {
			if (propertyJson["name"] == "ysort" && propertyJson["value"] == true) {
				layer.sortWithPlayer = true;
			}
		}

		if (layerJson["type"] == "tilelayer") {
			layer.tileNumbers = layerJson["data"].get<std::vector<int>>();
		}
		else if (layerJson["type"] == "objectgroup") {
			layer.isTileLayer = false;
			for (const nlohmann::json& objectJson : layerJson["objects"]) {
				MapObject object;
				object.name = objectJson["name"].get<std::string>();
				object.type = objectJson.value("type", "");
				object.position = sf::Vector2f(objectJson["x"].get<float>(), objectJson["y"].get<float>());
				if (objectJson.contains("gid")) {
					object.tileNumber = objectJson["gid"].get<int>();
					object.position.y -= objectJson["height"].get<float>();
				}
				layer.objects.push_back(object);
			}
		}
		else {
			continue; 
		}
		layers_.push_back(layer);
	}
	
	
}

bool TileMap::isSolidTile(int tileX, int tileY) const

{
	// outside the map
	const bool isOutsideMap = tileX < 0 || tileY < 0 || tileX >= widthInTiles_ || tileY >= heightInTiles_;
	if (isOutsideMap) {
		return true;
	}


	// the top-most tile decides, grass on top of water is walkable, water alone is not
	for (int layerIndex = static_cast<int>(layers_.size()) - 1; layerIndex >= 0; --layerIndex) {
		if (!layers_[layerIndex].isTileLayer) {
			continue; // skip object layers
		}
		const int tileNumber = layers_[layerIndex].tileNumbers[tileY * widthInTiles_ + tileX];
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
		if (tilesets_[tilesetIndex].firstTileNumber > tileNumber) {
			break;
		}
		foundIndex = tilesetIndex;
	}
	return foundIndex;
}

