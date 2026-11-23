#include "application/shared/modes/ModeManager.hpp"

#include <stdexcept>

void ModeManager::RegisterMode(Mode mode, IMode& implementation) {
    const auto [iterator, inserted] = modes.emplace(mode, &implementation);

    if (!inserted) {
        throw std::invalid_argument("ModeManager: Mode is already registered.");
    }
}

void ModeManager::SetMode(Mode mode) {
    const auto iterator = modes.find(mode);

    if (iterator == modes.end()) {
        throw std::invalid_argument("ModeManager: Mode is not registered.");
    }

    if (active == iterator->second) {
        return;
    }

    if (active != nullptr) {
        active->OnExit();
    }

    active = iterator->second;
    activeMode = mode;
    active->OnEnter();
}

ModeManager::Mode ModeManager::GetActiveMode() const noexcept {
    return activeMode;
}

bool ModeManager::IsActive(Mode mode) const noexcept {
    return active != nullptr && activeMode == mode;
}

bool ModeManager::HandleEvent(const sf::Event& event) {
    return active != nullptr && active->HandleEvent(event);
}

void ModeManager::Update(float deltaTime) {
    if (active != nullptr) {
        active->Update(deltaTime);
    }
}

void ModeManager::Render(sf::RenderWindow& window) {
    if (active != nullptr) {
        active->Render(window);
    }
}
