#pragma once

#include "presentation/shared/screens/Screen.hpp"

#include <functional>

class LoadGameScreen : public Screen {
    public:
        explicit LoadGameScreen(std::function<void()> onBack);

        LoadGameScreen(const LoadGameScreen&) = delete;
        LoadGameScreen& operator=(const LoadGameScreen&) = delete;
};
