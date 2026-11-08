#include "presentation/widgets/menu/MenuContainer.hpp"

MenuContainer::MenuContainer(float paddingAmount, std::unique_ptr<Widget> child, MenuContainerStyle style)
    : Widget(::children(std::move(child))), paddingAmount(paddingAmount), style(std::move(style))
{
}

sf::Vector2f MenuContainer::getSize() const
{
	const auto size = children.front()->getSize();

	return {size.x + paddingAmount * 2.f, size.y + paddingAmount * 2.f};
}

void MenuContainer::draw(sf::RenderTarget &target, sf::Vector2f position)
{
	const sf::Vector2f size = style.fillParent ? sf::Vector2f{target.getSize()} : getSize();

	if (style.background) {
		sf::RectangleShape background({size.x, size.y});
		background.setPosition(position);
		background.setFillColor(*style.background);
		background.setOutlineThickness(0.f);
		target.draw(background);
	}

	if (style.border) {
		sf::RectangleShape border({size.x, size.y});
		border.setPosition(position);
		border.setFillColor(sf::Color::Transparent);
		border.setOutlineColor(*style.border);
		border.setOutlineThickness(style.borderThickness);
		target.draw(border);
	}

	children.front()->draw(target, {position.x + paddingAmount, position.y + paddingAmount});
}
