#include "application/Application.hpp"

#include <SFML/System/Clock.hpp>

namespace {
    constexpr unsigned WindowWidth = 1280;
    constexpr unsigned WindowHeight = 720;
    constexpr unsigned FramerateLimit = 60;
    constexpr const char* WindowTitle = "RPG";
    constexpr const char* GenevaFontPath = "assets/fonts/Geneva.ttf";
}

Application::Application()
    : window(sf::VideoMode({WindowWidth, WindowHeight}), WindowTitle),
      renderer(assets),
      bootMode(renderer,
               {.onStartGame = [this] { modeManager.SetMode(ModeManager::Mode::Overworld); },
                .onExit = [this] { window.close(); }}),
      overworldMode(world, assets, renderer,
                    {.onStartBattle = [this] { modeManager.SetMode(ModeManager::Mode::Battle); },
                     .onExitToMainMenu = [this] { modeManager.SetMode(ModeManager::Mode::Boot); }}),
      battleMode(world, renderer) {
    window.setFramerateLimit(FramerateLimit);
    assets.LoadFont(FontId::Geneva, GenevaFontPath);

    RegisterModes();
}

void Application::RegisterModes() {
    modeManager.RegisterMode(ModeManager::Mode::Boot, bootMode);
    modeManager.RegisterMode(ModeManager::Mode::Overworld, overworldMode);
    modeManager.RegisterMode(ModeManager::Mode::Battle, battleMode);

    modeManager.SetMode(ModeManager::Mode::Boot);
}

void Application::Run() {
    sf::Clock frameClock;

    while (window.isOpen()) {
        HandleEvents();

        modeManager.Update(frameClock.restart().asSeconds());

        if (window.isOpen()) {
            modeManager.Render(window);
        }
    }
}

void Application::HandleEvents() {
    while (const auto event = window.pollEvent()) {
        if (event->is<sf::Event::Closed>()) {
            window.close();
            continue;
        }
        modeManager.HandleEvent(*event);
    }
}
