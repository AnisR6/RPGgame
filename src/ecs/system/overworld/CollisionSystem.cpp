#include "ecs/system/overworld/CollisionSystem.hpp"

#include "presentation/rendering/AssetManager.hpp"

#include <cmath>

namespace Engine::ECS {

    bool CollisionSystem::isBlocked(const TileMapData& tileMap, sf::Vector2f boxPosition,
                                    const CollisionComponent& collision) {
        // the cells of the box's 4 corners (pixels / tile size, rounded down)
        const float tileSize = static_cast<float>(tileMap.tileSizeInPixels);
        const float boxSize = collision.boxSizeInPixels;
        const int left = static_cast<int>(std::floor(boxPosition.x / tileSize));
        const int right = static_cast<int>(std::floor((boxPosition.x + boxSize - 1.0f) / tileSize));
        const int top = static_cast<int>(std::floor(boxPosition.y / tileSize));
        const int bottom = static_cast<int>(std::floor((boxPosition.y + boxSize - 1.0f) / tileSize));

        // one map lookup per corner, no loop over entities
        return tileMap.IsSolidTile(left, top) || tileMap.IsSolidTile(right, top) ||
               tileMap.IsSolidTile(left, bottom) || tileMap.IsSolidTile(right, bottom);
    }

}