#pragma once

#include "application/boot/BootCallbacks.hpp"
#include "application/shared/modes/IMode.hpp"
#include "presentation/boot/screens/LoadGameScreen.hpp"
#include "presentation/boot/screens/MainMenuScreen.hpp"
#include "presentation/shared/rendering/Renderer.hpp"

class BootMode : public IMode {
    public:
        BootMode(Renderer& renderer, BootCallbacks callbacks);

        void OnEnter() override;
        bool HandleEvent(const sf::Event& event) override;
        void Render(sf::RenderWindow& window) override;

    private:
        Renderer& renderer;
        MainMenuScreen mainMenuScreen;
        LoadGameScreen loadGameScreen;
        Screen* activeScreen;
};
