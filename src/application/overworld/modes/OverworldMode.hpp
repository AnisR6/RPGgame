#pragma once

#include "application/overworld/bus/OverworldMessageBus.hpp"
#include "application/overworld/callbacks/OverworldCallbacks.hpp"
#include "application/overworld/controllers/OverworldController.hpp"
#include "application/shared/modes/IMode.hpp"
#include "ecs/core/world/World.hpp"
#include "ecs/overworld/systems/OverworldState.hpp"
#include "ecs/overworld/systems/OverworldSystemManager.hpp"
#include "presentation/overworld/screens/OverworldScreen.hpp"

#include <SFML/System/Vector2.hpp>

class AssetManager;
class Renderer;

class OverworldMode : public IMode {
    public:
        OverworldMode(Engine::ECS::World& world, AssetManager& assets, Renderer& renderer,
                      OverworldCallbacks callbacks);

        void OnEnter() override;
        bool HandleEvent(const sf::Event& event) override;
        void Update(float deltaTime) override;
        void Render(sf::RenderWindow& window) override;

    private:
        void SubscribeToEvents();
        void SpawnEnemies(AssetManager& assets);
        void AddInGameBootMenu();

        void SetInGameBootMenuOpen(bool open);
        [[nodiscard]] bool IsInGameBootMenuOpen() const;

        void HandleOverworldInfoRequest() const;

        sf::Vector2f FindPlayerSpawnPoint() const;
        sf::Vector2f FindPlayerStartPosition() const;

        OverworldMessageBus messageBus;
        OverworldScreen screen;
        Engine::ECS::World& world;
        OverworldCallbacks callbacks;
        OverworldState state;
        OverworldSystemManager systemManager;
        Engine::ECS::Entity mapEntity;
        Engine::ECS::Entity playerEntity;
        OverworldController controller;
};
