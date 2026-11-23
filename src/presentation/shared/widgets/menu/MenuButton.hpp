#pragma once

#include "presentation/shared/widgets/core/Widget.hpp"

#include <functional>
#include <memory>
#include <utility>

class MenuButton : public Widget {
    private:
        std::function<void()> callback;
        bool selected = false;

    public:
        MenuButton(std::function<void()> callback, std::unique_ptr<Widget> child);

        void Activate() const;

        sf::Vector2f GetSize() const override;

        void Draw(sf::RenderTarget& target, sf::Vector2f position) override;

        void SetSelected(const bool value) { selected = value; }

        [[nodiscard]] std::function<void()> GetCallback() const { return callback; }
};

inline std::unique_ptr<MenuButton> button(std::function<void()> callback,
                                          std::unique_ptr<Widget> child) {
    return std::make_unique<MenuButton>(std::move(callback), std::move(child));
}

inline std::function<void()> onPressed(std::function<void()> callback) {
    return callback;
}
