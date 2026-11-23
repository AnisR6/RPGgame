
#pragma once

#include <SFML/Graphics.hpp>

#include <set>
#include <string>
#include <vector>

// -----------------------------------------------------------------------------
// Tileset Data
// -----------------------------------------------------------------------------
struct Tileset {
        int firstTileNumber = 0;
        int columnCount = 0;
        int squareWidth = 32;
        int squareHeight = 32;
        std::string imageFile;
        std::string name;
};

// -----------------------------------------------------------------------------
// Map Object Data
// -----------------------------------------------------------------------------
struct MapObject {
        int tileNumber = 0;
        std::string name;
        std::string type;
        sf::Vector2f position;
};

// -----------------------------------------------------------------------------
// Map Layer Data
// -----------------------------------------------------------------------------
struct MapLayer {
        std::string name;
        bool isTileLayer = true;
        bool sortWithPlayer = false;
        std::vector<int> tileNumbers;
        std::vector<MapObject> objects;
};

// -----------------------------------------------------------------------------
// Tile Map Loading Data
// -----------------------------------------------------------------------------
class TileMap {
    public:
        TileMap(const std::string& mapFilePath);

        bool isSolidTile(int tileX, int tileY) const;
        bool IsSolidTileNumber(int tileNumber) const;

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

        std::vector<MapLayer> layers_;
        std::set<int> solidTileNumbers_;
        std::vector<Tileset> tilesets_;
        std::vector<MapObject> objects_;
};
