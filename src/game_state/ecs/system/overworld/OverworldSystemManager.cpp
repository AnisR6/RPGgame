#include "game_state/ecs/system/overworld/OverworldSystemManager.hpp"

#include "game_state/ecs/system/overworld/AnimationSystem.hpp"
#include "game_state/ecs/system/overworld/CollisionSystem.hpp"
#include "game_state/ecs/system/overworld/InputSystem.hpp"
#include "game_state/ecs/system/overworld/MapInitializationSystem.hpp"
#include "game_state/ecs/system/overworld/MovementSystem.hpp"

namespace Engine::ECS {

    OverworldSystemManager::OverworldSystemManager(AssetManager& assets, std::string mapFilePath,
                                                    std::string tilesetFolderPath, sf::Vector2f playerStart,
                                                    std::string playerSpritePath)
    {
        RegisterSystem<MapInitializationSystem>(assets, std::move(mapFilePath), std::move(tilesetFolderPath),
                                                 playerStart, std::move(playerSpritePath));
        RegisterSystem<InputSystem>();
        RegisterSystem<AnimationSystem>();
        RegisterSystem<MovementSystem>();
        RegisterSystem<CollisionSystem>();
    }

} // namespace Engine::ECS
