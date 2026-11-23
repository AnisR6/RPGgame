#include "presentation/widgets/menu/MenuButton.hpp"

MenuButton::MenuButton(std::function<void()> callback, std::unique_ptr<Widget> child)
    : Widget(::children(std::move(child))), callback(std::move(callback))
{
}

void MenuButton::activate() const
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
	if (selected) {
		sf::RectangleShape highlight(getSize());
		highlight.setPosition(position);
		highlight.setFillColor(sf::Color(80, 100, 160, 100));
		highlight.setOutlineColor(sf::Color(140, 170, 255));
		highlight.setOutlineThickness(2.f);

		target.draw(highlight);
	}
	children.front()->draw(target, position);
}
