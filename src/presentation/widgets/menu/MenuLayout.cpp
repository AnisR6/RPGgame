#include "presentation/widgets/menu/MenuLayout.hpp"

#include <algorithm>

MenuLayout::MenuLayout(std::vector<std::unique_ptr<Widget>> incomingChildren, float spacing,
                       MenuOrientation orientation)
    : Widget(std::move(incomingChildren)), spacing(spacing), orientation(orientation)
{
}

sf::Vector2f MenuLayout::getSize() const
{
	sf::Vector2f result{};

	for (const auto &child : children) {
		const auto size = child->getSize();

		if (orientation == MenuOrientation::Column) {
			result.x = std::max(result.x, size.x);
			result.y += size.y;
		} else {
			result.x += size.x;
			result.y = std::max(result.y, size.y);
		}
	}

	if (children.size() > 1) {
		const float gap = spacing * static_cast<float>(children.size() - 1);

		if (orientation == MenuOrientation::Column) {
			result.y += gap;
		} else {
			result.x += gap;
		}
	}

	return result;
}

void MenuLayout::draw(sf::RenderTarget &target, sf::Vector2f position)
{
	for (const auto &child : children) {
		child->draw(target, position);

		const auto size = child->getSize();

		if (orientation == MenuOrientation::Column) {
			position.y += size.y + spacing;
		} else {
			position.x += size.x + spacing;
		}
	}
}
