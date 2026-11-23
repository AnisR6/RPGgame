#include "presentation/rendering/Renderer.hpp"

#include "ecs/components/overworld/InputComponent.hpp"
#include "ecs/components/overworld/LayerComponent.hpp"
#include "presentation/widgets/menu/MenuText.hpp"

#include <algorithm>
#include <array>
#include <utility>

using namespace Engine::ECS;

namespace {

    // -----------------------------------------------------------------------------
    // Render Ordering
    // -----------------------------------------------------------------------------
    constexpr std::array<Renderer::RenderLayer, 4> renderLayerOrder = {
        Renderer::RenderLayer::WorldLayers, Renderer::RenderLayer::HeadUpDisplayLayer,
        Renderer::RenderLayer::InGameMenuLayer, Renderer::RenderLayer::PauseMenuLayer};

    // -----------------------------------------------------------------------------
    // World Entity Sorting
    // -----------------------------------------------------------------------------
    struct ThingToDraw {
            float bottom = 0.0f;
            Entity entity;
    };

} // namespace

// -----------------------------------------------------------------------------
// Construction
// -----------------------------------------------------------------------------
Renderer::Renderer(AssetManager& assets, sf::Color clearColor)
    : assets(assets),
      clearColor(clearColor) {}

// -----------------------------------------------------------------------------
// Screen Rendering
// -----------------------------------------------------------------------------
void Renderer::Render(sf::RenderWindow& window, Screen& screen, sf::Vector2f position) {
    window.clear(clearColor);
    Draw(window, screen.getRootWidget(), position);
    window.display();
}

// -----------------------------------------------------------------------------
// World Configuration
// -----------------------------------------------------------------------------
void Renderer::SetWorld(World& world, const TileMapData& tileMap,
                        const std::string& tilesetFolderPath) {
    this->world = &world;
    this->tileMap = &tileMap;

    tilesetTextures.clear();
    tilesetTextures.reserve(tileMap.tilesets.size());

    for (const Tileset& tileset : tileMap.tilesets) {
        tilesetTextures.push_back(assets.LoadTexture(tilesetFolderPath + "/" + tileset.imageFile));
    }

    sortedLayerIndex = static_cast<int>(tileMap.layers.size()) - 1;

    for (int layerIndex = 0; layerIndex < static_cast<int>(tileMap.layers.size()); ++layerIndex) {
        if (tileMap.layers[layerIndex].sortWithPlayer) {
            sortedLayerIndex = layerIndex;
            break;
        }
    }
}

// -----------------------------------------------------------------------------
// Camera
// -----------------------------------------------------------------------------
sf::Vector2f Renderer::GetCameraCenter(sf::Vector2f viewSize) const {
    sf::Vector2f center;

    if (world == nullptr || tileMap == nullptr) {
        return center;
    }

    auto controlled =
        world->CreateQuery<Including<InputComponent, PositionComponent, SpriteComponent>>();

    controlled.ForEach([&](Entity, InputComponent&, PositionComponent& position,
                           SpriteComponent& sprite) {
        center = position.position + sprite.drawOffset +
                 sf::Vector2f(sprite.frameWidthInPixels / 2.0f, sprite.frameHeightInPixels / 2.0f);
    });

    const float mapWidth = static_cast<float>(tileMap->widthInTiles * tileMap->tileSizeInPixels);

    const float mapHeight = static_cast<float>(tileMap->heightInTiles * tileMap->tileSizeInPixels);

    if (mapWidth <= viewSize.x) {
        center.x = mapWidth / 2.0f;
    } else {
        center.x = std::clamp(center.x, viewSize.x / 2.0f, mapWidth - viewSize.x / 2.0f);
    }

    if (mapHeight <= viewSize.y) {
        center.y = mapHeight / 2.0f;
    } else {
        center.y = std::clamp(center.y, viewSize.y / 2.0f, mapHeight - viewSize.y / 2.0f);
    }

    return center;
}

// -----------------------------------------------------------------------------
// Layer Rendering
// -----------------------------------------------------------------------------
void Renderer::RenderLayers(sf::RenderTarget& target, sf::Vector2f cameraCenter,
                            sf::Vector2f viewSize) {
    const sf::View screenView = target.getView();

    sf::View worldView;
    worldView.setSize(viewSize);
    worldView.setCenter(cameraCenter);

    target.setView(worldView);
    DrawWorldLayers(target);

    target.setView(screenView);

    for (const RenderLayer layer : renderLayerOrder) {
        if (layer == RenderLayer::WorldLayers) {
            continue;
        }

        DrawWidgetLayers(target, layer);
    }
}

void Renderer::AddWidgetLayer(RenderLayer layer, std::unique_ptr<Widget> root) {
    widgetLayers.push_back({layer, std::move(root)});
}

// -----------------------------------------------------------------------------
// World Rendering
// -----------------------------------------------------------------------------
void Renderer::DrawWorldLayers(sf::RenderTarget& target) {
    if (world == nullptr || tileMap == nullptr || tileMap->layers.empty()) {
        return;
    }

    for (int worldLayerIndex = 0; worldLayerIndex < static_cast<int>(tileMap->layers.size());
         ++worldLayerIndex) {
        DrawWorldLayer(target, worldLayerIndex);
    }
}

void Renderer::DrawWorldLayer(sf::RenderTarget& target, int worldLayerIndex) {
    const MapLayer& layer = tileMap->layers[worldLayerIndex];

    if (layer.isTileLayer && layer.name != "Collision") {
        DrawTileLayer(target, layer);
    }

    DrawWorldEntities(target, worldLayerIndex);
}

// -----------------------------------------------------------------------------
// World Entity Rendering
// -----------------------------------------------------------------------------
void Renderer::DrawWorldEntities(sf::RenderTarget& target, int worldLayerIndex) {
    if (world == nullptr || tileMap == nullptr) {
        return;
    }

    std::vector<ThingToDraw> thingsToDraw;

    auto drawable = world->CreateQuery<Including<SpriteComponent, PositionComponent>>();

    drawable.ForEach([&](Entity entity, SpriteComponent& sprite, PositionComponent& position) {
        if (!IsSpriteOnScreen(target, sprite, position)) {
            return;
        }

        int entityLayerIndex = sortedLayerIndex;

        if (world->HasComponent<LayerComponent>(entity)) {
            const int componentLayerIndex = world->GetComponent<LayerComponent>(entity).zIndex;

            if (componentLayerIndex >= 0 &&
                componentLayerIndex < static_cast<int>(tileMap->layers.size()) &&
                !tileMap->layers[componentLayerIndex].sortWithPlayer) {
                entityLayerIndex = componentLayerIndex;
            }
        }

        if (entityLayerIndex != worldLayerIndex) {
            return;
        }

        const float bottom = position.position.y + sprite.drawOffset.y + sprite.frameHeightInPixels;

        thingsToDraw.push_back({bottom, entity});
    });

    if (worldLayerIndex == sortedLayerIndex) {
        std::stable_sort(thingsToDraw.begin(), thingsToDraw.end(),
                         [](const ThingToDraw& first, const ThingToDraw& second) {
                             return first.bottom < second.bottom;
                         });
    }

    for (const ThingToDraw& thing : thingsToDraw) {
        DrawSprite(target, world->GetComponent<SpriteComponent>(thing.entity),
                   world->GetComponent<PositionComponent>(thing.entity));
    }
}

// -----------------------------------------------------------------------------
// Tile Rendering
// -----------------------------------------------------------------------------
void Renderer::DrawTileLayer(sf::RenderTarget& target, const MapLayer& layer) {
    const int tileSize = tileMap->tileSizeInPixels;
    const int widthInTiles = tileMap->widthInTiles;
    const int heightInTiles = tileMap->heightInTiles;

    const sf::View& camera = target.getView();

    const sf::Vector2f viewTopLeft = camera.getCenter() - camera.getSize() / 2.0f;

    const sf::Vector2f viewBottomRight = camera.getCenter() + camera.getSize() / 2.0f;

    const int firstTileX = std::max(0, static_cast<int>(viewTopLeft.x) / tileSize);

    const int firstTileY = std::max(0, static_cast<int>(viewTopLeft.y) / tileSize);

    const int lastTileX =
        std::min(widthInTiles - 1, static_cast<int>(viewBottomRight.x) / tileSize);

    const int lastTileY =
        std::min(heightInTiles - 1, static_cast<int>(viewBottomRight.y) / tileSize);

    for (int tileY = firstTileY; tileY <= lastTileY; ++tileY) {
        for (int tileX = firstTileX; tileX <= lastTileX; ++tileX) {
            const int tileNumber = layer.tileNumbers[tileY * widthInTiles + tileX];

            if (tileNumber == 0) {
                continue;
            }

            const int tilesetIndex = tileMap->FindTilesetIndex(tileNumber);

            if (tilesetIndex < 0 || tilesetIndex >= static_cast<int>(tilesetTextures.size()) ||
                tilesetTextures[tilesetIndex] == nullptr) {
                continue;
            }

            const Tileset& tileset = tileMap->tilesets[tilesetIndex];

            if (tileset.columnCount <= 0) {
                continue;
            }

            const int squareIndexInPng = tileNumber - tileset.firstTileNumber;

            const int squareColumn = squareIndexInPng % tileset.columnCount;

            const int squareRow = squareIndexInPng / tileset.columnCount;

            const sf::IntRect squareInPng({squareColumn * tileSize, squareRow * tileSize},
                                          {tileSize, tileSize});

            sf::Sprite tileSprite(*tilesetTextures[tilesetIndex], squareInPng);

            tileSprite.setPosition(sf::Vector2f(static_cast<float>(tileX * tileSize),
                                                static_cast<float>(tileY * tileSize)));

            target.draw(tileSprite);
        }
    }
}

// -----------------------------------------------------------------------------
// Sprite Rendering
// -----------------------------------------------------------------------------
void Renderer::DrawSprite(sf::RenderTarget& target, const SpriteComponent& sprite,
                          const PositionComponent& position) {
    if (sprite.spriteSheet == nullptr) {
        return;
    }

    if (sprite.framesPerRow <= 0 || sprite.frameWidthInPixels <= 0 ||
        sprite.frameHeightInPixels <= 0) {
        return;
    }

    sf::Sprite frameSprite(*sprite.spriteSheet,
                           GetSpriteFrameRectangle(sprite, sprite.currentFrameNumber));

    frameSprite.setPosition(position.position + sprite.drawOffset);

    target.draw(frameSprite);
}

sf::IntRect Renderer::GetSpriteFrameRectangle(const SpriteComponent& sprite,
                                              int frameNumber) const {
    const int safeFramesPerRow = std::max(1, sprite.framesPerRow);

    const int column = frameNumber % safeFramesPerRow;
    const int row = frameNumber / safeFramesPerRow;

    const sf::Vector2i topLeftCorner(column * sprite.frameWidthInPixels,
                                     row * sprite.frameHeightInPixels);

    const sf::Vector2i frameSize(sprite.frameWidthInPixels, sprite.frameHeightInPixels);

    return sf::IntRect(topLeftCorner, frameSize);
}

// -----------------------------------------------------------------------------
// Camera Culling
// -----------------------------------------------------------------------------
bool Renderer::IsSpriteOnScreen(const sf::RenderTarget& target, const SpriteComponent& sprite,
                                const PositionComponent& position) const {
    const sf::View& camera = target.getView();

    const sf::FloatRect cameraArea(camera.getCenter() - camera.getSize() / 2.0f, camera.getSize());

    const sf::FloatRect pictureArea(position.position + sprite.drawOffset,
                                    sf::Vector2f(static_cast<float>(sprite.frameWidthInPixels),
                                                 static_cast<float>(sprite.frameHeightInPixels)));

    return cameraArea.findIntersection(pictureArea).has_value();
}

// -----------------------------------------------------------------------------
// Widget Layers
// -----------------------------------------------------------------------------
void Renderer::DrawWidgetLayers(sf::RenderTarget& target, RenderLayer layer) {
    for (WidgetLayer& widgetLayer : widgetLayers) {
        if (widgetLayer.layer == layer && widgetLayer.root) {
            Draw(target, *widgetLayer.root);
        }
    }
}

// -----------------------------------------------------------------------------
// Widget Rendering
// -----------------------------------------------------------------------------
void Renderer::Draw(sf::RenderTarget& target, Widget& rootWidget, sf::Vector2f position) {
    BindFonts(rootWidget);
    rootWidget.draw(target, position);
}

// -----------------------------------------------------------------------------
// Font Binding
// -----------------------------------------------------------------------------
void Renderer::BindFonts(Widget& widget) {
    if (auto* menuText = dynamic_cast<MenuText*>(&widget)) {
        menuText->setFont(assets.GetFont(menuText->getFontId()));
    }

    for (const auto& child : widget.getChildren()) {
        BindFonts(*child);
    }
}