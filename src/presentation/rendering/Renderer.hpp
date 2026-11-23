#pragma once

#include "ecs/components/overworld/PositionComponent.hpp"
#include "ecs/components/overworld/SpriteComponent.hpp"
#include "ecs/world/World.hpp"
#include "presentation/rendering/AssetManager.hpp"
#include "presentation/screens/core/Screen.hpp"
#include "presentation/widgets/core/Widget.hpp"

#include <SFML/Graphics.hpp>

#include <memory>
#include <string>
#include <vector>

// -----------------------------------------------------------------------------
// Renderer
// -----------------------------------------------------------------------------
class Renderer {
    public:
        // -------------------------------------------------------------------------
        // Render Layer
        // -------------------------------------------------------------------------
        enum class RenderLayer { WorldLayers, HeadUpDisplayLayer, InGameMenuLayer, PauseMenuLayer };

        // -------------------------------------------------------------------------
        // Construction
        // -------------------------------------------------------------------------
        explicit Renderer(AssetManager& assets, sf::Color clearColor = sf::Color{8, 10, 16});

        // -------------------------------------------------------------------------
        // Screen Rendering
        // -------------------------------------------------------------------------
        void Render(sf::RenderWindow& window, Screen& screen, sf::Vector2f position = {});

        // -------------------------------------------------------------------------
        // World Configuration
        // -------------------------------------------------------------------------
        void SetWorld(Engine::ECS::World& world, const TileMapData& tileMap,
                      const std::string& tilesetFolderPath);

        // -------------------------------------------------------------------------
        // Camera
        // -------------------------------------------------------------------------
        sf::Vector2f GetCameraCenter(sf::Vector2f viewSize) const;

        // -------------------------------------------------------------------------
        // Layer Rendering
        // -------------------------------------------------------------------------
        void RenderLayers(sf::RenderTarget& target, sf::Vector2f cameraCenter,
                          sf::Vector2f viewSize);

        void AddWidgetLayer(RenderLayer layer, std::unique_ptr<Widget> root);

        // -------------------------------------------------------------------------
        // Sprite Rendering
        // -------------------------------------------------------------------------
        void DrawSprite(sf::RenderTarget& target, const SpriteComponent& sprite,
                        const PositionComponent& position);

        // -------------------------------------------------------------------------
        // Widget Rendering
        // -------------------------------------------------------------------------
        void Draw(sf::RenderTarget& target, Widget& rootWidget, sf::Vector2f position = {});

    private:
        // -------------------------------------------------------------------------
        // Widget Layer
        // -------------------------------------------------------------------------
        struct WidgetLayer {
                RenderLayer layer;
                std::unique_ptr<Widget> root;
        };

        // -------------------------------------------------------------------------
        // World Rendering
        // -------------------------------------------------------------------------
        void DrawWorldLayers(sf::RenderTarget& target);

        void DrawWorldLayer(sf::RenderTarget& target, int worldLayerIndex);

        void DrawTileLayer(sf::RenderTarget& target, const MapLayer& layer);

        void DrawWorldEntities(sf::RenderTarget& target, int worldLayerIndex);

        // -------------------------------------------------------------------------
        // Sprite Frames
        // -------------------------------------------------------------------------
        sf::IntRect GetSpriteFrameRectangle(const SpriteComponent& sprite, int frameNumber) const;

        // -------------------------------------------------------------------------
        // Camera and Culling
        // -------------------------------------------------------------------------
        bool IsSpriteOnScreen(const sf::RenderTarget& target, const SpriteComponent& sprite,
                              const PositionComponent& position) const;

        // -------------------------------------------------------------------------
        // Widget Layers
        // -------------------------------------------------------------------------
        void DrawWidgetLayers(sf::RenderTarget& target, RenderLayer layer);

        // -------------------------------------------------------------------------
        // Font Binding
        // -------------------------------------------------------------------------
        void BindFonts(Widget& widget);

        // -------------------------------------------------------------------------
        // Dependencies
        // -------------------------------------------------------------------------
        AssetManager& assets;
        Engine::ECS::World* world = nullptr;
        const TileMapData* tileMap = nullptr;

        // -------------------------------------------------------------------------
        // Rendering Data
        // -------------------------------------------------------------------------
        sf::Color clearColor;
        std::vector<const sf::Texture*> tilesetTextures;
        std::vector<WidgetLayer> widgetLayers;

        int sortedLayerIndex = 0;
};