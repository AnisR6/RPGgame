#pragma once

#include "application/combat/modes/BattleMode.hpp"
#include "application/boot/BootMode.hpp"
#include "application/shared/modes/ModeManager.hpp"
#include "application/overworld/modes/OverworldMode.hpp"
#include "ecs/core/world/World.hpp"
#include "presentation/shared/rendering/AssetManager.hpp"
#include "presentation/shared/rendering/Renderer.hpp"

#include <SFML/Graphics/RenderWindow.hpp>

class Application {
    public:
        Application();

        void Run();

    private:
        void RegisterModes();
        void HandleEvents();

        sf::RenderWindow window;
        AssetManager assets;
        Renderer renderer;
        Engine::ECS::World world;
        ModeManager modeManager;
        BootMode bootMode;
        OverworldMode overworldMode;
        BattleMode battleMode;
};
