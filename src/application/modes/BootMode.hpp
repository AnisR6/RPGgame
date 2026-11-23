#pragma once

#include "presentation/rendering/Renderer.hpp"
#include "presentation/screens/boot_screen/MainMenuScreen.hpp"

#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Window/Event.hpp>

// -----------------------------------------------------------------------------
// BootMode
// -----------------------------------------------------------------------------

class BootMode {
    public:
        // -------------------------------------------------------------------------
        // Construction
        // -------------------------------------------------------------------------

        explicit BootMode(BootMenuCallbacks callbacks);

        // -------------------------------------------------------------------------
        // Event Handling
        // -------------------------------------------------------------------------

        bool HandleEvent(const sf::Event& event);

        // -------------------------------------------------------------------------
        // Rendering
        // -------------------------------------------------------------------------

        void Render(Renderer& renderer, sf::RenderWindow& window);

    private:
        // -------------------------------------------------------------------------
        // Presentation
        // -------------------------------------------------------------------------

        MainMenuScreen screen;
};
