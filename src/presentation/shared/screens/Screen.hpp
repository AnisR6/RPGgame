#pragma once

#include "presentation/shared/widgets/core/Widget.hpp"

#include <memory>
#include <stdexcept>
#include <utility>

class Screen {
    protected:
        std::unique_ptr<Widget> rootWidget;

    public:
        explicit Screen(std::unique_ptr<Widget> rootWidget)
            : rootWidget(std::move(rootWidget)) {
            if (!this->rootWidget) {
                throw std::invalid_argument("Screen requires a root widget");
            }
        }

        virtual ~Screen() = default;

        [[nodiscard]] Widget& GetRootWidget() { return *rootWidget; }

        [[nodiscard]] const Widget& GetRootWidget() const { return *rootWidget; }

        virtual bool HandleEvent(const sf::Event& event) {
            auto [handled, action] = rootWidget->HandleEvent(event);
            if (action) {
                action();
            }
            return handled;
        }

        void SetRootWidget(std::unique_ptr<Widget> newRoot) {
            if (!newRoot) {
                throw std::invalid_argument("Screen requires a valid root widget!");
            }
            rootWidget = std::move(newRoot);
        }
};
