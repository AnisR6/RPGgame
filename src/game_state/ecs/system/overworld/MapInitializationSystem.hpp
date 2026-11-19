#pragma once

#include "game_state/ecs/system/ISystem.hpp"

#include <SFML/System/Vector2.hpp>

#include <string>

class AssetManager;

namespace Engine::ECS {

    class MapInitializationSystem : public ISystem {
        public:
            MapInitializationSystem(AssetManager& assets, std::string mapFilePath, std::string tilesetFolderPath,
                                    sf::Vector2f playerStart, std::string playerSpritePath);

            void Update(SystemContext& context, float deltaTime) override;

        private:
            AssetManager& assets;
            std::string mapFilePath;
            std::string tilesetFolderPath;
            sf::Vector2f playerStart;
            std::string playerSpritePath;
            bool loaded = false;
    };

} // namespace Engine::ECS
