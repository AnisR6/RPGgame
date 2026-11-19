#pragma once

#include "presentation/widgets/core/Widget.hpp"

#include <cstddef>
#include <memory>
#include <vector>

namespace Engine::ECS {
	class World;
}

// A forest of entity widgets. Each root is one z-index layer, lowest first.
class EntityWidgetTree {
  public:
	void sync(Engine::ECS::World &world);

	void draw(sf::RenderTarget &target) const;

  private:
	std::vector<std::unique_ptr<Widget>> layers;
	std::size_t syncedSpriteCount = 0;
};
