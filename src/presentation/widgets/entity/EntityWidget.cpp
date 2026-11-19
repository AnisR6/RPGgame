#include "presentation/widgets/entity/EntityWidget.hpp"

#include "game_state/ecs/components/PositionComponent.hpp"
#include "game_state/ecs/components/SpriteComponent.hpp"
#include "game_state/ecs/world/World.hpp"
#include "presentation/rendering/SpriteRenderer.hpp"

EntityWidget::EntityWidget(Engine::ECS::World &world, Engine::ECS::Entity entity) : world(&world), entity(entity) {}

sf::Vector2f EntityWidget::getSize() const
{
	if (world == nullptr || !world->IsAlive(entity) || !world->HasComponent<SpriteComponent>(entity)) {
		return {};
	}

	const SpriteComponent &sprite = world->GetComponent<SpriteComponent>(entity);
	return {static_cast<float>(sprite.frameWidthInPixels), static_cast<float>(sprite.frameHeightInPixels)};
}

void EntityWidget::draw(sf::RenderTarget &target, sf::Vector2f)
{
	if (world == nullptr || !world->IsAlive(entity) || !world->HasComponent<SpriteComponent>(entity) ||
	    !world->HasComponent<PositionComponent>(entity)) {
		return;
	}

	SpriteRenderer::draw(target, world->GetComponent<SpriteComponent>(entity),
	                     world->GetComponent<PositionComponent>(entity));
}
