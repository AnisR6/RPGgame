#pragma once

#include "ecs/system/ISystem.hpp"

#include <SFML/System/Vector2.hpp>

#include <string>

class AssetManager;
class TileMap;

namespace Engine::ECS {

    // On the first update, makes the entities of the map:
    // every picture of the object layers (tree, house, sheep, foam...), every map point (NPC, save point...) and the player.
    // The ground (water, sand, grass, cliffs) is not made of entities: it stays in the TileMap.
    class MapInitializationSystem : public ISystem {
        public:
            MapInitializationSystem(const TileMap& tileMap, AssetManager& assets, std::string tilesetFolderPath,
                                    sf::Vector2f playerStart, std::string playerSpritePath);

            void Update(SystemContext& context, float deltaTime) override;

        private:
            const TileMap& tileMap;
            AssetManager& assets;
            std::string tilesetFolderPath;
            sf::Vector2f playerStart; // used if the map has no "PlayerSpawn" point
            std::string playerSpritePath;
            bool loaded = false;
    };

} // namespace Engine::ECS
