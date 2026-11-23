#pragma once

#include "presentation/shared/widgets/core/Widget.hpp"

#include <functional>
#include <memory>

struct InGameBootMenuCallbacks {
        std::function<void()> onResume;
        std::function<void()> onExit;
};

std::unique_ptr<Widget> CreateInGameBootMenu(InGameBootMenuCallbacks callbacks);
