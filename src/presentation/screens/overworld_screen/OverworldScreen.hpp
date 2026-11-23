#pragma once

#include "ecs/system/overworld/OverworldSystemManager.hpp"
#include "ecs/world/World.hpp"
#include "presentation/rendering/AssetManager.hpp"
#include "presentation/rendering/Renderer.hpp"
#include "presentation/widgets/core/Widget.hpp"

#include <SFML/Graphics/RenderTarget.hpp>

#include <memory>

// The overworld presentation
class OverworldScreen {
    public:
        OverworldScreen();

        void update(float secondsSinceLastFrame);

        void render(sf::RenderTarget& target);

        void addWidgetLayer(Renderer::RenderLayer layer, std::unique_ptr<Widget> root);

    private:
        // the order matters: each one is built after the ones it uses
        AssetManager assets;
        const TileMapData* tileMap = nullptr;
        Engine::ECS::World world;
        Engine::ECS::OverworldSystemManager systems;
        Renderer renderer;
};
