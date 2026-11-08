#pragma once

#include <SFML/Graphics.hpp>
#include <set>
#include <string>
#include <vector>

// one tileset, one PNG picture that Tiled cut into squares
struct Tileset {
	int firstTileNumber = 0; // the first map number that belongs to this PNG ("firstgid" in Tiled)
	int columnCount = 0;     // how many squares are in one row of the PNG
	sf::Texture picture;     // the PNG itself
};

class TileMap {

  public:
	TileMap(const std::string &mapFilePath, const std::string &tilesetFolderPath);

	void render(sf::RenderTarget &target) const;

	bool isSolidTile(int tileX, int tileY) const;
	int getTileSizeInPixels() const { return tileSizeInPixels_; }
	int getWidthInTiles() const { return widthInTiles_; }
	int getHeightInTiles() const { return heightInTiles_; }

  private:
	int findTilesetIndex(int tileNumber) const;

	int widthInTiles_ = 0;
	int heightInTiles_ = 0;
	int tileSizeInPixels_ = 32;
	std::vector<std::vector<int>> tileLayers_; // one list of tile numbers per tile layer, bottom layer first
	std::set<int> solidTileNumbers_;           // tile numbers marked "solid" in Tiled
	std::vector<Tileset> tilesets_;
};