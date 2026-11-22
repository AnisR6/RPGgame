#pragma once

#include "MenuButton.hpp"
#include "presentation/widgets/core/Widget.hpp"

#include <memory>
#include <utility>

class Menu : public Widget {
  private:
	std::size_t selectedIndex = 0;
	[[nodiscard]] std::vector<MenuButton *> getButtons() const;

  public:
	Menu() = default;

	explicit Menu(std::unique_ptr<Widget> content);

	[[nodiscard]] sf::Vector2f getSize() const override;

	void draw(sf::RenderTarget &target, sf::Vector2f position) override;

	EventResult handleEvent(const sf::Event &event) override;

	void updateSelection() const;
};

inline std::unique_ptr<Menu> menu(std::unique_ptr<Widget> content = nullptr)
{
	return std::make_unique<Menu>(std::move(content));
}
