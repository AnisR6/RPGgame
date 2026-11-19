#pragma once

#include "game_state/ecs/system/overworld/OverworldSystemManager.hpp"
#include "game_state/ecs/world/World.hpp"
#include "presentation/layers/PresentationLayers.hpp"
#include "presentation/rendering/AssetManager.hpp"
#include "presentation/widgets/core/Widget.hpp"

#include <SFML/Graphics/RenderTarget.hpp>

#include <memory>

// The overworld presentation: entity layers from the world, then widget layers on top.
class OverworldScreen {
  public:
	OverworldScreen();

	void update(float secondsSinceLastFrame);

	void render(sf::RenderTarget &target) const;

	void addWidgetLayer(int zIndex, std::unique_ptr<Widget> root);

  private:
	AssetManager assets;
	Engine::ECS::World world;
	Engine::ECS::OverworldSystemManager systems;
	PresentationLayers presentation;
};
