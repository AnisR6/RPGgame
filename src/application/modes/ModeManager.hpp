#pragma once

#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Window/Event.hpp>

#include <functional>
#include <unordered_map>

// -----------------------------------------------------------------------------
// ModeManager
// -----------------------------------------------------------------------------

class ModeManager {
    public:
        // -------------------------------------------------------------------------
        // Mode Definition
        // -------------------------------------------------------------------------

        enum class Mode { Boot, Overworld, Battle };

        // -------------------------------------------------------------------------
        // Mode Callbacks
        // -------------------------------------------------------------------------

        struct ModeCallbacks {
                std::function<void()> onEnter;
                std::function<void()> onExit;
                std::function<bool(const sf::Event&)> handleEvent;
                std::function<void(float)> update;
                std::function<void(sf::RenderWindow&)> render;
        };

        // -------------------------------------------------------------------------
        // Mode Registration
        // -------------------------------------------------------------------------

        void RegisterMode(Mode mode, ModeCallbacks callbacks);

        // -------------------------------------------------------------------------
        // Mode Switching
        // -------------------------------------------------------------------------

        void SetMode(Mode mode);

        // -------------------------------------------------------------------------
        // Active Mode
        // -------------------------------------------------------------------------

        [[nodiscard]] Mode GetActiveMode() const noexcept;

        [[nodiscard]] bool IsActive(Mode mode) const noexcept;

        // -------------------------------------------------------------------------
        // Mode Dispatch
        // -------------------------------------------------------------------------

        bool HandleEvent(const sf::Event& event);

        void Update(float deltaTime);

        void Render(sf::RenderWindow& window);

    private:
        // -------------------------------------------------------------------------
        // Storage
        // -------------------------------------------------------------------------

        std::unordered_map<Mode, ModeCallbacks> modes;
        Mode activeMode = Mode::Boot;
        bool hasActiveMode = false;
};
