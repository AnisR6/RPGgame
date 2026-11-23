#pragma once

#include "presentation/screens/core/Screen.hpp"

#include <functional>

// -----------------------------------------------------------------------------
// BootMenuCallbacks
// -----------------------------------------------------------------------------

struct BootMenuCallbacks {
        std::function<void()> onStartGame;
        std::function<void()> onExit;
};

// -----------------------------------------------------------------------------
// MainMenuScreen
// -----------------------------------------------------------------------------

class MainMenuScreen : public Screen {
    public:
        // -------------------------------------------------------------------------
        // Construction
        // -------------------------------------------------------------------------

        explicit MainMenuScreen(BootMenuCallbacks callbacks);

        MainMenuScreen(const MainMenuScreen&) = delete;
        MainMenuScreen& operator=(const MainMenuScreen&) = delete;
};
