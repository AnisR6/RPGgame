
#pragma once

#include "ecs/system/ISystem.hpp"

#include <SFML/System/Vector2.hpp>

#include <string>

class AssetManager;
struct TileMapData;

namespace Engine::ECS {

    // -----------------------------------------------------------------------------
    // Map Initialization System
    // -----------------------------------------------------------------------------
    class MapInitializationSystem : public ISystem {
        public:
            // -------------------------------------------------------------------------
            // Construction
            // -------------------------------------------------------------------------
            MapInitializationSystem(const TileMapData& tileMap, AssetManager& assets,
                                    std::string tilesetFolderPath, sf::Vector2f playerStart,
                                    std::string playerSpritePath);

            // -------------------------------------------------------------------------
            // System Update
            // -------------------------------------------------------------------------
            void Update(SystemContext& context, float deltaTime) override;

        private:
            // -------------------------------------------------------------------------
            // Dependencies
            // -----------------------------------------------------------------------------
            const TileMapData& tileMap;
            AssetManager& assets;

            // -------------------------------------------------------------------------
            // Initialization Data
            // -----------------------------------------------------------------------------
            std::string tilesetFolderPath;
            sf::Vector2f playerStart;
            std::string playerSpritePath;
            bool loaded = false;
    };

} // namespace Engine::ECS
