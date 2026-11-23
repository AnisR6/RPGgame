#include "ecs/system/overworld/OverworldSystemManager.hpp"

#include "ecs/system/overworld/AnimationSystem.hpp"
#include "ecs/system/overworld/InputSystem.hpp"
#include "ecs/system/overworld/MapInitializationSystem.hpp"
#include "ecs/system/overworld/MovementSystem.hpp"

namespace Engine::ECS {

    OverworldSystemManager::OverworldSystemManager(const TileMap& tileMap, AssetManager& assets,
                                                   std::string tilesetFolderPath, sf::Vector2f playerStart,
                                                   std::string playerSpritePath)
    {
        // the order here is the order they run every frame
        RegisterSystem<MapInitializationSystem>(tileMap, assets, std::move(tilesetFolderPath), playerStart,
                                                std::move(playerSpritePath));
        RegisterSystem<InputSystem>();
        RegisterSystem<AnimationSystem>();
        RegisterSystem<MovementSystem>(tileMap);
    }

} // namespace Engine::ECS
