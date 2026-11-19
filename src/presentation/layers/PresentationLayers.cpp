#include "presentation/layers/PresentationLayers.hpp"

#include "game_state/ecs/components/InputComponent.hpp"
#include "game_state/ecs/components/PositionComponent.hpp"
#include "game_state/ecs/world/World.hpp"

#include <algorithm>
#include <utility>

void PresentationLayers::sync(Engine::ECS::World &world)
{
	entityTree.sync(world);

	cameraCenter_.reset();
	auto controlled = world.CreateQuery<Engine::ECS::Including<PositionComponent, InputComponent>>();
	controlled.ForEach([this](Engine::ECS::Entity, PositionComponent &position, InputComponent &) {
		cameraCenter_ = position.position;
	});
}

void PresentationLayers::addWidgetLayer(int zIndex, std::unique_ptr<Widget> root)
{
	const auto insertAt = std::find_if(widgetLayers.begin(), widgetLayers.end(),
	                                    [zIndex](const WidgetLayer &layer) { return layer.zIndex > zIndex; });
	widgetLayers.insert(insertAt, WidgetLayer{zIndex, std::move(root)});
}

void PresentationLayers::drawEntities(sf::RenderTarget &target) const
{
	entityTree.draw(target);
}

void PresentationLayers::drawWidgetLayers(sf::RenderTarget &target) const
{
	for (const WidgetLayer &layer : widgetLayers) {
		if (layer.root) {
			layer.root->draw(target, {});
		}
	}
}
