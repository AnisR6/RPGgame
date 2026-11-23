#include "application/boot/BootMode.hpp"

#include <utility>

BootMode::BootMode(Renderer& renderer, BootCallbacks callbacks)
    : renderer(renderer),
      mainMenuScreen({.onStartGame = std::move(callbacks.onStartGame),
                      .onLoadGame = [this] { activeScreen = &loadGameScreen; },
                      .onExit = std::move(callbacks.onExit)}),
      loadGameScreen([this] { activeScreen = &mainMenuScreen; }),
      activeScreen(&mainMenuScreen) {}

void BootMode::OnEnter() {
    activeScreen = &mainMenuScreen;
}

bool BootMode::HandleEvent(const sf::Event& event) {
    return activeScreen->HandleEvent(event);
}

void BootMode::Render(sf::RenderWindow& window) {
    renderer.Render(window, *activeScreen);
}
