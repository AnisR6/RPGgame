#pragma once

#include "presentation/widgets/core/Widget.hpp"

#include <functional>
#include <memory>
#include <utility>

class MenuButton : public Widget {
  private:
	std::function<void()> callback;

  public:
	MenuButton(std::function<void()> callback, std::unique_ptr<Widget> child);

	void activate();

	sf::Vector2f getSize() const override;

	void draw(sf::RenderTarget &target, sf::Vector2f position) override;
};

inline std::unique_ptr<MenuButton> button(std::function<void()> callback, std::unique_ptr<Widget> child)
{
	return std::make_unique<MenuButton>(std::move(callback), std::move(child));
}

inline std::function<void()> onPressed(std::function<void()> callback)
{
	return callback;
}
