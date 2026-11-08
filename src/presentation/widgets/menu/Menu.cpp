#include "presentation/widgets/menu/Menu.hpp"

Menu::Menu(std::unique_ptr<Widget> content)
    : Widget(content ? ::children(std::move(content)) : std::vector<std::unique_ptr<Widget>>{})
{
}

sf::Vector2f Menu::getSize() const
{
	if (children.empty()) {
		return {};
	}

	return children.front()->getSize();
}

void Menu::draw(sf::RenderTarget &target, sf::Vector2f position)
{
	if (children.empty()) {
		return;
	}

	children.front()->draw(target, position);
}
