#pragma once

#include "game_state/ecs/entity/Entity.hpp"
#include "presentation/widgets/core/Widget.hpp"

namespace Engine::ECS {
	class World;
}

// Draws one entity from its sprite and position components.
class EntityWidget : public Widget {
  public:
	EntityWidget(Engine::ECS::World &world, Engine::ECS::Entity entity);

	sf::Vector2f getSize() const override;

	void draw(sf::RenderTarget &target, sf::Vector2f position) override;

  private:
	Engine::ECS::World *world = nullptr;
	Engine::ECS::Entity entity;
};
