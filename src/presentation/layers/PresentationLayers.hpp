#pragma once

#include "persistence/Tilemap.hpp"
#include "presentation/widgets/core/Widget.hpp"

#include <SFML/Graphics/RenderTarget.hpp>
#include <SFML/Graphics/Texture.hpp>

#include <memory>
#include <string>
#include <vector>

class AssetManager;

namespace Engine::ECS {
	class World;
}

struct WidgetLayer {
	int zIndex = 0;
	std::unique_ptr<Widget> root;
};

// Draws the overworld layer by layer like in Tiled (ground from the TileMap, pictures and player from the entities),
// then the menus on top.
class PresentationLayers {
public:
	PresentationLayers(const TileMap& tileMap, Engine::ECS::World& world, AssetManager& assets,
		const std::string& tilesetFolderPath);

	void addWidgetLayer(int zIndex, std::unique_ptr<Widget> root);

	void drawWorld(sf::RenderTarget& target) const;
	void drawWidgetLayers(sf::RenderTarget& target) const;

	sf::Vector2f cameraCenter(sf::Vector2f viewSize) const; // the player, but never past the edge of the map

private:
	void drawTileLayer(sf::RenderTarget& target, const MapLayer& layer) const;

	const TileMap& tileMap;
	Engine::ECS::World& world;
	std::vector<const sf::Texture*> tilesetTextures; // one per tileset (nullptr: PNG missing)
	int sortedLayerIndex = 0; // the first "ysort" layer: trees, houses and the player are drawn there
	std::vector<WidgetLayer> widgetLayers;
};
