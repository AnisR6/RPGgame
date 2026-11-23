#include "ecs/overworld/systems/TileCollision.hpp"

#include <cmath>

namespace {
    bool IsSolidCell(const MapComponent& map, int tileX, int tileY) {
        const bool isOutsideMap =
            tileX < 0 || tileY < 0 || tileX >= map.widthInTiles || tileY >= map.heightInTiles;

        if (isOutsideMap) {
            return true;
        }

        return map.solidCells[tileY * map.widthInTiles + tileX] != 0;
    }
}

namespace TileCollision {
    bool IsBlocked(const MapComponent& map, sf::Vector2f boxPosition,
                   const CollisionComponent& collision) {
        const float tileSize = static_cast<float>(map.tileSizeInPixels);
        const float boxSize = collision.boxSizeInPixels;
        const int left = static_cast<int>(std::floor(boxPosition.x / tileSize));
        const int right = static_cast<int>(std::floor((boxPosition.x + boxSize - 1.0f) / tileSize));
        const int top = static_cast<int>(std::floor(boxPosition.y / tileSize));
        const int bottom =
            static_cast<int>(std::floor((boxPosition.y + boxSize - 1.0f) / tileSize));

        return IsSolidCell(map, left, top) || IsSolidCell(map, right, top) ||
               IsSolidCell(map, left, bottom) || IsSolidCell(map, right, bottom);
    }
}
