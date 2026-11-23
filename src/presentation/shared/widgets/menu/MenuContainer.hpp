#pragma once

#include "presentation/shared/widgets/core/Widget.hpp"

#include <SFML/Graphics.hpp>

#include <memory>
#include <optional>
#include <utility>

struct MenuContainerStyle {
        std::optional<sf::Color> background;
        std::optional<sf::Color> border;
        float borderThickness = 1.f;
        bool fillParent = false;
};

class MenuContainer : public Widget {
    private:
        float paddingAmount;
        MenuContainerStyle style;

    public:
        MenuContainer(float paddingAmount, std::unique_ptr<Widget> child,
                      MenuContainerStyle style = {});

        sf::Vector2f GetSize() const override;

        void Draw(sf::RenderTarget& target, sf::Vector2f position) override;
};

inline std::unique_ptr<MenuContainer> padding(float amount, std::unique_ptr<Widget> child,
                                              MenuContainerStyle style = {}) {
    return std::make_unique<MenuContainer>(amount, std::move(child), style);
}
