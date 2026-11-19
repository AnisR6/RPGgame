#include "game_state/ecs/system/overworld/CollisionSystem.hpp"

#include "game_state/ecs/components/MapBoundsComponent.hpp"
#include "game_state/ecs/components/PositionComponent.hpp"
#include "game_state/ecs/query/Query.hpp"

namespace Engine::ECS {

    namespace {

        bool isSolidTile(SystemContext& context, int tileX, int tileY, int widthInTiles, int heightInTiles,
                         int tileSizeInPixels)
        {
            const bool isOutsideMap =
                tileX < 0 || tileY < 0 || tileX >= widthInTiles || tileY >= heightInTiles;
            if (isOutsideMap) {
                return true;
            }

            bool solid = false;
            const float tileSize = static_cast<float>(tileSizeInPixels);

            auto tiles = context.CreateQuery<Including<PositionComponent, CollisionComponent>>();
            tiles.ForEach([&](Entity, PositionComponent& position, CollisionComponent& tileCollision) {
                if (!tileCollision.solid || solid) {
                    return;
                }

                const int tileColumn = static_cast<int>(position.position.x / tileSize);
                const int tileRow = static_cast<int>(position.position.y / tileSize);
                if (tileColumn == tileX && tileRow == tileY) {
                    solid = true;
                }
            });

            return solid;
        }

    } // namespace

    void CollisionSystem::Update(SystemContext&, float) {}

    bool CollisionSystem::isBlocked(SystemContext& context, sf::Vector2f boxPosition,
                                    const CollisionComponent& collision)
    {
        int widthInTiles = 0;
        int heightInTiles = 0;
        int tileSizeInPixels = 32;

        auto boundsQuery = context.CreateQuery<Including<MapBoundsComponent>>();
        boundsQuery.ForEach([&](Entity, MapBoundsComponent& bounds) {
            widthInTiles = bounds.widthInTiles;
            heightInTiles = bounds.heightInTiles;
            tileSizeInPixels = bounds.tileSizeInPixels;
        });

        const float tileSize = static_cast<float>(tileSizeInPixels);

        // pixels -> tiles: divide by the tile size, drop the decimals. Check the box's 4 corners.
        const int leftTileX = static_cast<int>(boxPosition.x / tileSize);
        const int rightTileX =
            static_cast<int>((boxPosition.x + collision.boxSizeInPixels - 1.0f) / tileSize);
        const int topTileY = static_cast<int>(boxPosition.y / tileSize);
        const int bottomTileY =
            static_cast<int>((boxPosition.y + collision.boxSizeInPixels - 1.0f) / tileSize);

        return isSolidTile(context, leftTileX, topTileY, widthInTiles, heightInTiles, tileSizeInPixels) ||
               isSolidTile(context, rightTileX, topTileY, widthInTiles, heightInTiles, tileSizeInPixels) ||
               isSolidTile(context, leftTileX, bottomTileY, widthInTiles, heightInTiles, tileSizeInPixels) ||
               isSolidTile(context, rightTileX, bottomTileY, widthInTiles, heightInTiles, tileSizeInPixels);
    }

} // namespace Engine::ECS
