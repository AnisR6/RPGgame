#include "application/overworld/modes/OverworldMode.hpp"

#include "application/overworld/mappers/OverworldMapper.hpp"
#include "ecs/overworld/components/CollisionComponent.hpp"
#include "ecs/overworld/components/MapPointComponent.hpp"
#include "ecs/overworld/components/PositionComponent.hpp"
#include "ecs/overworld/prefabs/OverworldEnemy.hpp"
#include "ecs/shared/prefabs/Player.hpp"
#include "presentation/overworld/menus/InGameBootMenu.hpp"
#include "presentation/shared/rendering/AssetManager.hpp"
#include "presentation/shared/rendering/Renderer.hpp"

#include <SFML/Graphics/Color.hpp>
#include <SFML/Window/Keyboard.hpp>

#include <array>
#include <utility>

namespace {
    constexpr const char* MapFilePath = "assets/maps/overworld1.json";
    constexpr const char* TilesetFolderPath = "assets/tilesets";
    constexpr const char* PlayerSpriteSheetPath = "assets/tilesets/Player.png";
    constexpr const char* PlayerSpawnPointName = "PlayerSpawn";
    constexpr sf::Vector2f DefaultPlayerSpawnPoint(652.0f, 588.0f);
    constexpr sf::Keyboard::Key OpenInGameBootMenuKey = sf::Keyboard::Key::Escape;

    struct EnemyPlacement {
            const char* name;
            sf::Vector2f offsetFromPlayerSpawn;
            sf::Color tint;
    };

    constexpr std::array EnemyPlacements = {
        EnemyPlacement{"Slime", {96.0f, 0.0f}, sf::Color(90, 220, 90)},
        EnemyPlacement{"Goblin", {128.0f, 0.0f}, sf::Color(170, 140, 60)},
        EnemyPlacement{"Spider", {160.0f, 0.0f}, sf::Color(110, 60, 140)},
    };
}

OverworldMode::OverworldMode(Engine::ECS::World& world, AssetManager& assets, Renderer& renderer,
                             OverworldCallbacks callbacks)
    : screen(renderer),
      world(world),
      callbacks(std::move(callbacks)),
      systemManager(state),
      mapEntity(assets.LoadTileMap(MapFilePath, TilesetFolderPath, world)),
      playerEntity(Prefabs::CreateOverworldPlayer(world, FindPlayerStartPosition(),
                                                  assets.LoadTexture(PlayerSpriteSheetPath))),
      controller(messageBus, screen, world) {
    SpawnEnemies(assets);
    AddInGameBootMenu();
    SubscribeToEvents();
}

void OverworldMode::AddInGameBootMenu() {
    screen.AddWidgetLayer(
        OverworldScreen::WidgetLayer::InGameMenu,
        CreateInGameBootMenu({.onResume = [this] { SetInGameBootMenuOpen(false); },
                              .onExit =
                                  [this] {
                                      SetInGameBootMenuOpen(false);

                                      if (callbacks.onExitToMainMenu) {
                                          callbacks.onExitToMainMenu();
                                      }
                                  }}),
        false);
}

void OverworldMode::SetInGameBootMenuOpen(const bool open) {
    screen.SetWidgetLayerVisible(OverworldScreen::WidgetLayer::InGameMenu, open);
}

bool OverworldMode::IsInGameBootMenuOpen() const {
    return screen.IsWidgetLayerVisible(OverworldScreen::WidgetLayer::InGameMenu);
}

void OverworldMode::SubscribeToEvents() {
    messageBus.Subscribe<OverworldInfoRequest>(
        [this](const OverworldInfoRequest&) { HandleOverworldInfoRequest(); });
}

void OverworldMode::SpawnEnemies(AssetManager& assets) {
    const sf::Vector2f playerSpawnPoint = FindPlayerSpawnPoint();
    const sf::Texture* spriteSheet = assets.LoadTexture(PlayerSpriteSheetPath);

    for (const EnemyPlacement& placement : EnemyPlacements) {
        Prefabs::CreateOverworldEnemy(world, placement.name,
                                      playerSpawnPoint + placement.offsetFromPlayerSpawn,
                                      spriteSheet, placement.tint);
    }
}

void OverworldMode::OnEnter() {
    state.encounteredEnemy.reset();
    SetInGameBootMenuOpen(false);
    controller.RequestOverworldInfo();
}

bool OverworldMode::HandleEvent(const sf::Event& event) {
    if (IsInGameBootMenuOpen()) {
        return screen.HandleEvent(event);
    }

    const auto* keyPressed = event.getIf<sf::Event::KeyPressed>();

    if (keyPressed != nullptr && keyPressed->code == OpenInGameBootMenuKey) {
        SetInGameBootMenuOpen(true);
        return true;
    }

    return screen.HandleEvent(event);
}

void OverworldMode::Update(const float deltaTime) {
    if (IsInGameBootMenuOpen()) {
        return;
    }

    world.UpdateSystems(systemManager, deltaTime);

    if (state.encounteredEnemy && callbacks.onStartBattle) {
        state.encounteredEnemy.reset();
        callbacks.onStartBattle();
    }
}

void OverworldMode::Render(sf::RenderWindow& window) {
    window.clear();
    screen.Render(window);
    window.display();
}

void OverworldMode::HandleOverworldInfoRequest() const {
    messageBus.Publish(OverworldMapper::MapToOverworldInfoResponse(mapEntity, playerEntity));
}

sf::Vector2f OverworldMode::FindPlayerSpawnPoint() const {
    sf::Vector2f spawnPoint = DefaultPlayerSpawnPoint;

    auto mapPoints =
        world.CreateQuery<Engine::ECS::Including<MapPointComponent, PositionComponent>>();

    mapPoints.ForEach(
        [&](Engine::ECS::Entity, MapPointComponent& mapPoint, PositionComponent& position) {
            if (mapPoint.name == PlayerSpawnPointName) {
                spawnPoint = position.position;
            }
        });

    return spawnPoint;
}

sf::Vector2f OverworldMode::FindPlayerStartPosition() const {
    const float halfBox = CollisionComponent{}.boxSizeInPixels / 2.0f;
    return FindPlayerSpawnPoint() - sf::Vector2f(halfBox, halfBox);
}
