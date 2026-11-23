#pragma once
#include <functional>

struct EventResult {
        bool handled = false;
        std::function<void()> action;
};
