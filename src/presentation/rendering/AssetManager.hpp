#pragma once

#include <SFML/Graphics/Font.hpp>
#include <SFML/Graphics/Texture.hpp>
#include <SFML/System/Vector2.hpp>

#include <memory>
#include <set>
#include <string>
#include <unordered_map>
#include <vector>

// -----------------------------------------------------------------------------
// Font Identifier
// -----------------------------------------------------------------------------
enum class FontId { Geneva };

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
// Tile Map Data
// -----------------------------------------------------------------------------
struct TileMapData {
        int widthInTiles = 0;
        int heightInTiles = 0;
        int tileSizeInPixels = 32;

        std::vector<MapLayer> layers;
        std::set<int> solidTileNumbers;
        std::vector<Tileset> tilesets;
        std::vector<MapObject> objects;

        // -------------------------------------------------------------------------
        // Tile Collision
        // -------------------------------------------------------------------------
        bool IsSolidTile(int tileX, int tileY) const;
        bool IsSolidTileNumber(int tileNumber) const;

        // -------------------------------------------------------------------------
        // Tileset Lookup
        // -------------------------------------------------------------------------
        int FindTilesetIndex(int tileNumber) const;
};

// -----------------------------------------------------------------------------
// Asset Manager
// -----------------------------------------------------------------------------
class AssetManager {
    public:
        // -------------------------------------------------------------------------
        // Texture Loading
        // -------------------------------------------------------------------------
        const sf::Texture* LoadTexture(const std::string& path);

        // -------------------------------------------------------------------------
        // Font Loading
        // -------------------------------------------------------------------------
        void LoadFont(FontId id, const std::string& path);

        const sf::Font& GetFont(FontId id) const;
        bool HasFont(FontId id) const noexcept;

        // -------------------------------------------------------------------------
        // Tile Map Loading
        // -------------------------------------------------------------------------
        const TileMapData* LoadTileMap(const std::string& mapFilePath);

    private:
        // -------------------------------------------------------------------------
        // Asset Storage
        // -------------------------------------------------------------------------
        std::unordered_map<std::string, sf::Texture> textures;
        std::unordered_map<FontId, std::unique_ptr<sf::Font>> fonts;
        std::unordered_map<std::string, TileMapData> tileMaps;
};