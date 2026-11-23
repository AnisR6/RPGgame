#pragma once

#include "ecs/system/overworld/OverworldSystemManager.hpp"
#include "ecs/world/World.hpp"
#include "persistence/Tilemap.hpp"
#include "presentation/layers/PresentationLayers.hpp"
#include "presentation/rendering/AssetManager.hpp"
#include "presentation/widgets/core/Widget.hpp"

#include <SFML/Graphics/RenderTarget.hpp>

#include <memory>

// The overworld presentation
class OverworldScreen {
public:
	OverworldScreen();

	void update(float secondsSinceLastFrame);

	void render(sf::RenderTarget& target) const;

	void addWidgetLayer(int zIndex, std::unique_ptr<Widget> root);

private:
	// the order matters: each one is built after the ones it uses
	AssetManager assets;
	TileMap tileMap;
	Engine::ECS::World world;
	Engine::ECS::OverworldSystemManager systems;
	PresentationLayers presentation;
};
