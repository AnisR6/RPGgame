#include "game_logic/systems/game/overworld/collision_system/CollisionSystem.hpp"

// check if would the box at this position touch a wall,  we check its 4 corners
bool CollisionSystem::isBlocked(sf::Vector2f boxPosition, const CollisionComponent &collision, const TileMap &tileMap)
{
	const float tileSize = static_cast<float>(tileMap.getTileSizeInPixels());

	// pixels -> tiles: divide by the tile size, drop the decimals
	const int leftTileX = static_cast<int>(boxPosition.x / tileSize);
	const int rightTileX = static_cast<int>((boxPosition.x + collision.boxSizeInPixels - 1.0f) / tileSize);
	const int topTileY = static_cast<int>(boxPosition.y / tileSize);
	const int bottomTileY = static_cast<int>((boxPosition.y + collision.boxSizeInPixels - 1.0f) / tileSize);

	return tileMap.isSolidTile(leftTileX, topTileY) || tileMap.isSolidTile(rightTileX, topTileY)
	       || tileMap.isSolidTile(leftTileX, bottomTileY) || tileMap.isSolidTile(rightTileX, bottomTileY);
}