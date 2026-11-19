#include "presentation/widgets/entity/EntityWidgetTree.hpp"

#include "game_state/ecs/components/LayerComponent.hpp"
#include "game_state/ecs/components/PositionComponent.hpp"
#include "game_state/ecs/components/SpriteComponent.hpp"
#include "game_state/ecs/world/World.hpp"
#include "presentation/widgets/entity/EntityWidget.hpp"

#include <algorithm>
#include <map>
#include <utility>
#include <vector>

namespace {

class EntityLayerWidget : public Widget {
  public:
	sf::Vector2f getSize() const override { return {}; }

	void draw(sf::RenderTarget &target, sf::Vector2f position) override
	{
		for (const auto &child : children) {
			child->draw(target, position);
		}
	}
};

} // namespace

void EntityWidgetTree::sync(Engine::ECS::World &world)
{
	auto sprites = world.CreateQuery<Engine::ECS::Including<SpriteComponent, PositionComponent, LayerComponent>>();
	const std::size_t spriteCount = sprites.Count();
	if (spriteCount == syncedSpriteCount && (spriteCount == 0 || !layers.empty())) {
		return;
	}

	std::map<int, std::vector<Engine::ECS::Entity>> byZIndex;
	sprites.ForEach([&](Engine::ECS::Entity entity, SpriteComponent &, PositionComponent &, LayerComponent &layer) {
		byZIndex[layer.zIndex].push_back(entity);
	});

	layers.clear();
	for (auto &[zIndex, entities] : byZIndex) {
		(void)zIndex;
		std::sort(entities.begin(), entities.end(), [&world](Engine::ECS::Entity left, Engine::ECS::Entity right) {
			const PositionComponent &leftPosition = world.GetComponent<PositionComponent>(left);
			const PositionComponent &rightPosition = world.GetComponent<PositionComponent>(right);
			if (leftPosition.position.y != rightPosition.position.y) {
				return leftPosition.position.y < rightPosition.position.y;
			}
			return leftPosition.position.x < rightPosition.position.x;
		});

		auto layerWidget = std::make_unique<EntityLayerWidget>();
		for (const Engine::ECS::Entity entity : entities) {
			layerWidget->addChild(std::make_unique<EntityWidget>(world, entity));
		}
		layers.push_back(std::move(layerWidget));
	}

	syncedSpriteCount = spriteCount;
}

void EntityWidgetTree::draw(sf::RenderTarget &target) const
{
	for (const auto &layer : layers) {
		layer->draw(target, {});
	}
}
