#pragma once

#include <SFML/Graphics.hpp>
#include <set>
#include <string>
#include <vector>

// one tileset, one PNG picture that Tiled cut into squares
struct Tileset {
	int firstTileNumber = 0; // the first map number that belongs to this PNG ("firstgid" in Tiled)
	int columnCount = 0;     // how many squares are in one row of the PNG
	int squareWidth = 32;	 // to get the full size of PNG
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
	bool sortWithPlayer = false; // for objects only
	std::vector<int> tileNumbers; // for tile layers only
	std::vector<MapObject> objects; // for object layers only
};

class TileMap {

public:
	TileMap(const std::string& mapFilePath);


	bool isSolidTile(int tileX, int tileY) const;
	int getTileSizeInPixels() const { return tileSizeInPixels_; }
	int getWidthInTiles() const { return widthInTiles_; }
	int getHeightInTiles() const { return heightInTiles_; }
	int findTilesetIndex(int tileNumber) const;
	const std::vector<Tileset>& getTilesets() const { return tilesets_; }
	const std::vector<MapLayer>& getLayers() const { return layers_; }


private:

	int widthInTiles_ = 0;
	int heightInTiles_ = 0;
	int tileSizeInPixels_ = 32;
	std::vector<MapLayer> layers_; // one list of tile numbers per tile layer, bottom layer first
	std::set<int> solidTileNumbers_;           // tile numbers marked "solid" in Tiled
	std::vector<Tileset> tilesets_;
	std::vector<MapObject> objects_;

};