#pragma once

#include "ecs/core/world/World.hpp"
#include "presentation/shared/rendering/Renderer.hpp"
#include "presentation/shared/widgets/core/Widget.hpp"

#include <SFML/Graphics/RenderTarget.hpp>
#include <SFML/Window/Event.hpp>

#include <memory>
#include <vector>

class OverworldScreen {
    public:
        enum class WidgetLayer { HeadUpDisplay, InGameMenu, PauseMenu };

        explicit OverworldScreen(Renderer& renderer);

        void SetWorld(Engine::ECS::World& world);

        void Render(sf::RenderTarget& target);

        bool HandleEvent(const sf::Event& event);

        void AddWidgetLayer(WidgetLayer layer, std::unique_ptr<Widget> rootWidget,
                            bool visible = true);

        void SetWidgetLayerVisible(WidgetLayer layer, bool visible);

        [[nodiscard]] bool IsWidgetLayerVisible(WidgetLayer layer) const;

    private:
        struct LayeredWidget {
                WidgetLayer layer;
                std::unique_ptr<Widget> root;
                bool visible = true;
        };

        void DrawWidgetLayer(sf::RenderTarget& target, WidgetLayer layer);

        Renderer& renderer;
        Engine::ECS::World* world = nullptr;
        std::vector<LayeredWidget> layeredWidgets;
};
