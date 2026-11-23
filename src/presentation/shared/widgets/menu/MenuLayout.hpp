#pragma once

#include "presentation/shared/widgets/core/Widget.hpp"

#include <memory>
#include <vector>

enum class MenuOrientation { Column, Row };

class MenuLayout : public Widget {
    protected:
        float spacing;
        MenuOrientation orientation;

    public:
        MenuLayout(std::vector<std::unique_ptr<Widget>> incomingChildren, float spacing,
                   MenuOrientation orientation);

        sf::Vector2f GetSize() const override;

        void Draw(sf::RenderTarget& target, sf::Vector2f position) override;
};
