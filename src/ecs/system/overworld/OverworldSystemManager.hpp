#pragma once

#include "ecs/system/SystemManager.hpp"

#include <SFML/System/Vector2.hpp>

#include <string>

class AssetManager;
struct TileMapData;

namespace Engine::ECS {

    class OverworldSystemManager : public SystemManager {
        public:
            OverworldSystemManager(const TileMapData& tileMap, AssetManager& assets,
                                   std::string tilesetFolderPath,
                                   sf::Vector2f playerStart, std::string playerSpritePath);
    };

} // namespace Engine::ECS
