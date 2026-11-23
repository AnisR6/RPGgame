#pragma once

#include "presentation/shared/screens/Screen.hpp"

#include <functional>

struct BootMenuCallbacks {
        std::function<void()> onStartGame;
        std::function<void()> onLoadGame;
        std::function<void()> onExit;
};

class MainMenuScreen : public Screen {
    public:
        explicit MainMenuScreen(BootMenuCallbacks callbacks);

        MainMenuScreen(const MainMenuScreen&) = delete;
        MainMenuScreen& operator=(const MainMenuScreen&) = delete;
};
