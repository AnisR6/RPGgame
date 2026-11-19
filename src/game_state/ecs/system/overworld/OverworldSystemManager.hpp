#pragma once

#include "game_state/ecs/system/SystemManager.hpp"

#include <SFML/System/Vector2.hpp>

#include <string>

class AssetManager;

namespace Engine::ECS {

    class OverworldSystemManager : public SystemManager {
        public:
            OverworldSystemManager(AssetManager& assets, std::string mapFilePath, std::string tilesetFolderPath,
                                   sf::Vector2f playerStart, std::string playerSpritePath);
    };

} // namespace Engine::ECS
