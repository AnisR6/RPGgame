#pragma once

#include "presentation/widgets/core/Widget.hpp"

#include <memory>
#include <utility>

class Menu : public Widget {
  public:
	Menu() = default;

	explicit Menu(std::unique_ptr<Widget> content);

	sf::Vector2f getSize() const override;

	void draw(sf::RenderTarget &target, sf::Vector2f position) override;
};

inline std::unique_ptr<Menu> menu(std::unique_ptr<Widget> content = nullptr)
{
	return std::make_unique<Menu>(std::move(content));
}
