#pragma once

#include "presentation/shared/assets/AssetManager.hpp"
#include "presentation/shared/widgets/core/Widget.hpp"

#include <SFML/Graphics.hpp>

#include <memory>
#include <stdexcept>
#include <string>
#include <utility>

class MenuText : public Widget {
    private:
        FontId fontId;
        const sf::Font* font = nullptr;
        std::string value;
        unsigned int characterSize;
        sf::Color fillColor;

    public:
        MenuText(std::string value, unsigned int characterSize = 28,
                 sf::Color fillColor = sf::Color::White, FontId fontId = FontId::Geneva);

        void BindFonts(const AssetManager& assets) override;

        sf::Vector2f GetSize() const override;

        void Draw(sf::RenderTarget& target, sf::Vector2f position) override;
};

inline std::unique_ptr<MenuText> text(std::string value, unsigned int characterSize = 28,
                                      sf::Color fillColor = sf::Color::White,
                                      FontId fontId = FontId::Geneva) {
    return std::make_unique<MenuText>(std::move(value), characterSize, fillColor, fontId);
}
