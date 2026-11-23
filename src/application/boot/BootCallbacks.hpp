#pragma once

#include <functional>

struct BootCallbacks {
        std::function<void()> onStartGame;
        std::function<void()> onExit;
};
