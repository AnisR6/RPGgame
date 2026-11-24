#pragma once

#include "presentation/shared/assets/AssetManager.hpp"
#include "presentation/shared/widgets/core/Widget.hpp"

#include <SFML/Graphics/RenderTarget.hpp>
#include <SFML/System/Vector2.hpp>

class WidgetRenderer {
    public:
        explicit WidgetRenderer(const AssetManager& assets);

        void Draw(sf::RenderTarget& target, Widget& rootWidget, sf::Vector2f position = {});

    private:
        const AssetManager& assets;
};
