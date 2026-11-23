#include "presentation/layers/PresentationLayers.hpp"

#include "ecs/components/InputComponent.hpp"
#include "ecs/components/LayerComponent.hpp"
#include "ecs/components/PositionComponent.hpp"
#include "ecs/components/SpriteComponent.hpp"
#include "ecs/world/World.hpp"
#include "presentation/rendering/AssetManager.hpp"
#include "presentation/rendering/SpriteRenderer.hpp"

#include <SFML/Graphics/Sprite.hpp>

#include <algorithm>
#include <utility>

using namespace Engine::ECS;

namespace {

	struct ThingToDraw {
		float bottom = 0.0f; // y of its lowest pixel (the feet)
		Entity entity;
	};

	// culling: is any part of this picture inside the camera?
	bool isOnScreen(const sf::RenderTarget& target, const SpriteComponent& sprite, const PositionComponent& position)
	{
		const sf::View& camera = target.getView();
		const sf::FloatRect cameraArea(camera.getCenter() - camera.getSize() / 2.0f, camera.getSize());
		const sf::FloatRect pictureArea(position.position + sprite.drawOffset,
			sf::Vector2f(sprite.frameWidthInPixels, sprite.frameHeightInPixels));
		return cameraArea.findIntersection(pictureArea).has_value();
	}

} // namespace

PresentationLayers::PresentationLayers(const TileMap& tileMap, World& world, AssetManager& assets,
	const std::string& tilesetFolderPath)
	: tileMap(tileMap), world(world)
{
	for (const Tileset& tileset : tileMap.getTilesets()) {
		tilesetTextures.push_back(assets.loadTexture(tilesetFolderPath + "/" + tileset.imageFile));
	}

	const std::vector<MapLayer>& layers = tileMap.getLayers();
	sortedLayerIndex = static_cast<int>(layers.size()) - 1; // no "ysort" layer: on top
	for (int layerIndex = 0; layerIndex < static_cast<int>(layers.size()); ++layerIndex) {
		if (layers[layerIndex].sortWithPlayer) {
			sortedLayerIndex = layerIndex;
			break;
		}
	}
}

void PresentationLayers::addWidgetLayer(int zIndex, std::unique_ptr<Widget> root)
{
	const auto insertAt = std::find_if(widgetLayers.begin(), widgetLayers.end(),
		[zIndex](const WidgetLayer& layer) { return layer.zIndex > zIndex; });
	widgetLayers.insert(insertAt, WidgetLayer{ zIndex, std::move(root) });
}

void PresentationLayers::drawWorld(sf::RenderTarget& target) const
{
	const std::vector<MapLayer>& layers = tileMap.getLayers();
	if (layers.empty()) {
		return; // the map could not be loaded
	}

	// 1. put every entity the camera can see in the list of its layer.
	//    "ysort" pictures and characters (no LayerComponent) all go in the same list, to be sorted together.
	std::vector<std::vector<ThingToDraw>> thingsOfLayer(layers.size());

	auto drawable = world.CreateQuery<Including<SpriteComponent, PositionComponent>>();
	drawable.ForEach([&](Entity entity, SpriteComponent& sprite, PositionComponent& position) {
		if (!isOnScreen(target, sprite, position)) {
			return;
		}
		int layerIndex = sortedLayerIndex;
		if (world.HasComponent<LayerComponent>(entity) && !layers[world.GetComponent<LayerComponent>(entity).zIndex].sortWithPlayer) {
			layerIndex = world.GetComponent<LayerComponent>(entity).zIndex; // foam, shadows, clouds
		}
		const float bottom = position.position.y + sprite.drawOffset.y + sprite.frameHeightInPixels;
		thingsOfLayer[layerIndex].push_back(ThingToDraw{ bottom, entity });
		});

	// 2. trees, houses and the player: lower on the screen = drawn later = in front (3D effect)
	std::vector<ThingToDraw>& sortedThings = thingsOfLayer[sortedLayerIndex];
	std::stable_sort(sortedThings.begin(), sortedThings.end(),
		[](const ThingToDraw& a, const ThingToDraw& b) { return a.bottom < b.bottom; });

	// 3. draw layer by layer, like in Tiled
	for (int layerIndex = 0; layerIndex < static_cast<int>(layers.size()); ++layerIndex) {
		if (layers[layerIndex].isTileLayer && layers[layerIndex].name != "Collision") {
			drawTileLayer(target, layers[layerIndex]);
		}
		for (const ThingToDraw& thing : thingsOfLayer[layerIndex]) {
			SpriteRenderer::draw(target, world.GetComponent<SpriteComponent>(thing.entity),
				world.GetComponent<PositionComponent>(thing.entity));
		}
	}
}

// cutting the PNG into squares and drawing them at the right place on the map
void PresentationLayers::drawTileLayer(sf::RenderTarget& target, const MapLayer& layer) const
{
	const int tileSize = tileMap.getTileSizeInPixels();
	const int widthInTiles = tileMap.getWidthInTiles();

	// only the cells the camera can see
	const sf::View& camera = target.getView();
	const sf::Vector2f viewTopLeft = camera.getCenter() - camera.getSize() / 2.0f;
	const sf::Vector2f viewBottomRight = camera.getCenter() + camera.getSize() / 2.0f;
	const int firstTileX = std::max(0, static_cast<int>(viewTopLeft.x) / tileSize);
	const int firstTileY = std::max(0, static_cast<int>(viewTopLeft.y) / tileSize);
	const int lastTileX = std::min(widthInTiles - 1, static_cast<int>(viewBottomRight.x) / tileSize);
	const int lastTileY = std::min(tileMap.getHeightInTiles() - 1, static_cast<int>(viewBottomRight.y) / tileSize);

	for (int tileY = firstTileY; tileY <= lastTileY; ++tileY) {
		for (int tileX = firstTileX; tileX <= lastTileX; ++tileX) {
			const int tileNumber = layer.tileNumbers[tileY * widthInTiles + tileX];
			if (tileNumber == 0) {
				continue; // empty cell
			}

			// 1. which PNG
			const int tilesetIndex = tileMap.findTilesetIndex(tileNumber);
			if (tilesetTextures[tilesetIndex] == nullptr) {
				continue; // the PNG is missing
			}
			const Tileset& tileset = tileMap.getTilesets()[tilesetIndex];

			// 2. which square inside that PNG
			const int squareIndexInPng = tileNumber - tileset.firstTileNumber;
			const int squareColumn = squareIndexInPng % tileset.columnCount;
			const int squareRow = squareIndexInPng / tileset.columnCount;

			// 3. cut that square out of the PNG
			const sf::IntRect squareInPng({ squareColumn * tileSize, squareRow * tileSize }, { tileSize, tileSize });
			sf::Sprite tileSprite(*tilesetTextures[tilesetIndex], squareInPng);

			// 4. draw it at its place on the map
			tileSprite.setPosition(sf::Vector2f(static_cast<float>(tileX * tileSize), static_cast<float>(tileY * tileSize)));
			target.draw(tileSprite);
		}
	}
}

void PresentationLayers::drawWidgetLayers(sf::RenderTarget& target) const
{
	for (const WidgetLayer& layer : widgetLayers) {
		if (layer.root) {
			layer.root->draw(target, {});
		}
	}
}

sf::Vector2f PresentationLayers::cameraCenter(sf::Vector2f viewSize) const
{
	// the middle of the player's picture
	sf::Vector2f center;
	auto controlled = world.CreateQuery<Including<InputComponent, PositionComponent, SpriteComponent>>();
	controlled.ForEach([&](Entity, InputComponent&, PositionComponent& position, SpriteComponent& sprite) {
		center = position.position + sprite.drawOffset +
			sf::Vector2f(sprite.frameWidthInPixels / 2.0f, sprite.frameHeightInPixels / 2.0f);
		});

	// never show what is outside the map
	const float mapWidth = static_cast<float>(tileMap.getWidthInTiles() * tileMap.getTileSizeInPixels());
	const float mapHeight = static_cast<float>(tileMap.getHeightInTiles() * tileMap.getTileSizeInPixels());
	center.x = std::clamp(center.x, viewSize.x / 2.0f, mapWidth - viewSize.x / 2.0f);
	center.y = std::clamp(center.y, viewSize.y / 2.0f, mapHeight - viewSize.y / 2.0f);
	return center;
}
