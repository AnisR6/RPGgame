#pragma once

#include "presentation/rendering/FontManager.hpp"
#include "presentation/widgets/core/Widget.hpp"

#include <SFML/Graphics.hpp>

#include <memory>
#include <stdexcept>
#include <string>
#include <utility>

class MenuText : public Widget {
  private:
	FontId fontId;
	const sf::Font *font = nullptr;
	std::string value;
	unsigned int characterSize;
	sf::Color fillColor;

  public:
	MenuText(std::string value, unsigned int characterSize = 28, sf::Color fillColor = sf::Color::White,
	         FontId fontId = FontId::Geneva);

	void setFont(const sf::Font &resolvedFont);

	[[nodiscard]] FontId getFontId() const noexcept;

	[[nodiscard]] bool hasFont() const noexcept;

	sf::Vector2f getSize() const override;

	void draw(sf::RenderTarget &target, sf::Vector2f position) override;
};

inline std::unique_ptr<MenuText> text(std::string value, unsigned int characterSize = 28,
                                      sf::Color fillColor = sf::Color::White, FontId fontId = FontId::Geneva)
{
	return std::make_unique<MenuText>(std::move(value), characterSize, fillColor, fontId);
}
