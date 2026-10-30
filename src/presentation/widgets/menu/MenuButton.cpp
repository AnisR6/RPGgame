#include "presentation/widgets/menu/MenuButton.hpp"

MenuButton::MenuButton(std::function<void()> callback, std::unique_ptr<Widget> child)
    : Widget(::children(std::move(child))), callback(std::move(callback))
{
}

void MenuButton::activate()
{
	if (callback) {
		callback();
	}
}

sf::Vector2f MenuButton::getSize() const
{
	return children.front()->getSize();
}

void MenuButton::draw(sf::RenderTarget &target, sf::Vector2f position)
{
	children.front()->draw(target, position);
}
