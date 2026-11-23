#include "presentation/overworld/screens/OverworldScreen.hpp"

#include "presentation/overworld/rendering/Camera.hpp"

#include <array>
#include <ranges>
#include <utility>

namespace {
    constexpr std::array<OverworldScreen::WidgetLayer, 3> WidgetLayerOrder = {
        OverworldScreen::WidgetLayer::HeadUpDisplay, OverworldScreen::WidgetLayer::InGameMenu,
        OverworldScreen::WidgetLayer::PauseMenu};
}

OverworldScreen::OverworldScreen(Renderer& renderer)
    : renderer(renderer) {}

void OverworldScreen::SetWorld(Engine::ECS::World& world) {
    this->world = &world;
}

void OverworldScreen::Render(sf::RenderTarget& target) {
    if (world != nullptr) {
        const sf::View screenView = target.getView();

        target.setView(Camera::FollowPlayer(*world, target.getDefaultView().getSize()));
        renderer.DrawWorld(target, *world);
        target.setView(screenView);
    }

    for (const WidgetLayer layer : WidgetLayerOrder) {
        DrawWidgetLayer(target, layer);
    }
}

bool OverworldScreen::HandleEvent(const sf::Event& event) {
    for (const WidgetLayer layer : std::views::reverse(WidgetLayerOrder)) {
        for (LayeredWidget& layeredWidget : layeredWidgets) {
            if (layeredWidget.layer != layer || !layeredWidget.visible || !layeredWidget.root) {
                continue;
            }

            auto [handled, action] = layeredWidget.root->HandleEvent(event);

            if (action) {
                action();
            }

            if (handled) {
                return true;
            }
        }
    }

    return false;
}

void OverworldScreen::AddWidgetLayer(WidgetLayer layer, std::unique_ptr<Widget> rootWidget,
                                     bool visible) {
    layeredWidgets.push_back({layer, std::move(rootWidget), visible});
}

void OverworldScreen::SetWidgetLayerVisible(WidgetLayer layer, bool visible) {
    for (LayeredWidget& layeredWidget : layeredWidgets) {
        if (layeredWidget.layer == layer) {
            layeredWidget.visible = visible;
        }
    }
}

bool OverworldScreen::IsWidgetLayerVisible(WidgetLayer layer) const {
    for (const LayeredWidget& layeredWidget : layeredWidgets) {
        if (layeredWidget.layer == layer && layeredWidget.visible) {
            return true;
        }
    }

    return false;
}

void OverworldScreen::DrawWidgetLayer(sf::RenderTarget& target, WidgetLayer layer) {
    for (LayeredWidget& layeredWidget : layeredWidgets) {
        if (layeredWidget.layer == layer && layeredWidget.visible && layeredWidget.root) {
            renderer.DrawWidget(target, *layeredWidget.root);
        }
    }
}
