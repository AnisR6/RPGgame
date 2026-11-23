#pragma once

// -----------------------------------------------------------------------------
// Overworld Screen
// -----------------------------------------------------------------------------

#include "presentation/rendering/Renderer.hpp"
#include "presentation/widgets/core/Widget.hpp"

#include <SFML/Graphics/RenderTarget.hpp>

#include <memory>

class OverworldScreen {
    public:
        // -------------------------------------------------------------------------
        // Construction
        // -------------------------------------------------------------------------
        OverworldScreen();

        // -------------------------------------------------------------------------
        // Presentation
        // -------------------------------------------------------------------------
        void Render(sf::RenderTarget& target);

        // -------------------------------------------------------------------------
        // Widget Layer Management
        // -------------------------------------------------------------------------
        void AddWidgetLayer(RenderLayer renderLayer, std::unique_ptr<Widget> rootWidget);

    private:
        // -------------------------------------------------------------------------
        // Rendering
        // -------------------------------------------------------------------------
        Renderer renderer;
};