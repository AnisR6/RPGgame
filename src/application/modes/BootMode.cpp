#include "application/modes/BootMode.hpp"

#include <utility>

// -----------------------------------------------------------------------------
// Construction
// -----------------------------------------------------------------------------

BootMode::BootMode(BootMenuCallbacks callbacks)
    : screen(std::move(callbacks)) {}

// -----------------------------------------------------------------------------
// Event Handling
// -----------------------------------------------------------------------------

bool BootMode::HandleEvent(const sf::Event& event) {
    return screen.handleEvent(event);
}

// -----------------------------------------------------------------------------
// Rendering
// -----------------------------------------------------------------------------

void BootMode::Render(Renderer& renderer, sf::RenderWindow& window) {
    renderer.Render(window, screen);
}
