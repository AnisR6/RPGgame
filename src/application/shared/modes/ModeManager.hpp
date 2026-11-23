#pragma once

#include "application/shared/modes/IMode.hpp"

#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Window/Event.hpp>

#include <unordered_map>

class ModeManager {
    public:
        enum class Mode { Boot, Overworld, Battle };

        void RegisterMode(Mode mode, IMode& implementation);

        void SetMode(Mode mode);

        [[nodiscard]] Mode GetActiveMode() const noexcept;
        [[nodiscard]] bool IsActive(Mode mode) const noexcept;

        bool HandleEvent(const sf::Event& event);
        void Update(float deltaTime);
        void Render(sf::RenderWindow& window);

    private:
        std::unordered_map<Mode, IMode*> modes;
        IMode* active = nullptr;
        Mode activeMode = Mode::Boot;
};
