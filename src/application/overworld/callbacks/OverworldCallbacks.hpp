#pragma once

#include <functional>

struct OverworldCallbacks {
        std::function<void()> onStartBattle;
        std::function<void()> onExitToMainMenu;
};
