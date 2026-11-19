#pragma once

#include "presentation/widgets/entity/EntityWidgetTree.hpp"

#include <SFML/System/Vector2.hpp>

#include <memory>
#include <optional>
#include <vector>

namespace Engine::ECS {
	class World;
}

// A menu or other widget tree drawn after every entity layer.
struct WidgetLayer {
	int zIndex = 0;
	std::unique_ptr<Widget> root;
};

// Flattens the entity forest from back to front, then draws widget layers on top.
class PresentationLayers {
  public:
	void sync(Engine::ECS::World &world);

	void addWidgetLayer(int zIndex, std::unique_ptr<Widget> root);

	void drawEntities(sf::RenderTarget &target) const;

	void drawWidgetLayers(sf::RenderTarget &target) const;

	[[nodiscard]] const std::optional<sf::Vector2f> &cameraCenter() const { return cameraCenter_; }

  private:
	EntityWidgetTree entityTree;
	std::vector<WidgetLayer> widgetLayers;
	std::optional<sf::Vector2f> cameraCenter_;
};
