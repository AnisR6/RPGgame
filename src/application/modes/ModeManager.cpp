#include "application/modes/ModeManager.hpp"

#include <stdexcept>
#include <utility>

// -----------------------------------------------------------------------------
// Mode Registration
// -----------------------------------------------------------------------------

void ModeManager::RegisterMode(Mode mode, ModeCallbacks callbacks) {
    const auto [iterator, inserted] = modes.emplace(mode, std::move(callbacks));

    if (!inserted) {
        throw std::invalid_argument("ModeManager: Mode is already registered.");
    }
}

// -----------------------------------------------------------------------------
// Mode Switching
// -----------------------------------------------------------------------------

void ModeManager::SetMode(Mode mode) {
    const auto newModeIterator = modes.find(mode);

    if (newModeIterator == modes.end()) {
        throw std::invalid_argument("ModeManager: Mode is not registered.");
    }

    if (hasActiveMode && activeMode == mode) {
        return;
    }

    if (hasActiveMode) {
        const auto currentModeIterator = modes.find(activeMode);

        if (currentModeIterator != modes.end() && currentModeIterator->second.onExit) {
            currentModeIterator->second.onExit();
        }
    }

    activeMode = mode;
    hasActiveMode = true;

    if (newModeIterator->second.onEnter) {
        newModeIterator->second.onEnter();
    }
}

// -----------------------------------------------------------------------------
// Active Mode
// -----------------------------------------------------------------------------

ModeManager::Mode ModeManager::GetActiveMode() const noexcept {
    return activeMode;
}

bool ModeManager::IsActive(Mode mode) const noexcept {
    return hasActiveMode && activeMode == mode;
}

// -----------------------------------------------------------------------------
// Mode Dispatch
// -----------------------------------------------------------------------------

bool ModeManager::HandleEvent(const sf::Event& event) {
    if (!hasActiveMode) {
        return false;
    }

    const auto iterator = modes.find(activeMode);

    if (iterator == modes.end() || !iterator->second.handleEvent) {
        return false;
    }

    return iterator->second.handleEvent(event);
}

void ModeManager::Update(float deltaTime) {
    if (!hasActiveMode) {
        return;
    }

    const auto iterator = modes.find(activeMode);

    if (iterator == modes.end() || !iterator->second.update) {
        return;
    }

    iterator->second.update(deltaTime);
}

void ModeManager::Render(sf::RenderWindow& window) {
    if (!hasActiveMode) {
        return;
    }

    const auto iterator = modes.find(activeMode);

    if (iterator == modes.end() || !iterator->second.render) {
        return;
    }

    iterator->second.render(window);
}
