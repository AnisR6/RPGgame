#pragma once

#include <SFML/Graphics/Texture.hpp>

#include <string>
#include <unordered_map>
#include <vector>

// One visible tile. AssetManager has already picked the topmost tile in this cell.
struct TopTile {
	int tileX = 0;
	int tileY = 0;
	int zIndex = 0;
	int frameNumber = 0;
	int framesPerRow = 1;
	int frameSizeInPixels = 32;
	bool solid = false;
	const sf::Texture *texture = nullptr;
};

// The map after loading. Covered tiles from lower layers are not in here.
struct LoadedMap {
	int widthInTiles = 0;
	int heightInTiles = 0;
	int tileSizeInPixels = 32;
	int layerCount = 0;
	std::vector<TopTile> tiles;
};

// Owns textures and turns a Tiled map into the topmost tile of each cell.
class AssetManager {
  public:
	const sf::Texture &loadTexture(const std::string &path);

	// Reads the map file. Lower layers stay here only long enough to choose the top tile.
	LoadedMap loadMap(const std::string &mapFilePath, const std::string &tilesetFolderPath);

  private:
	std::unordered_map<std::string, sf::Texture> textures;
};
