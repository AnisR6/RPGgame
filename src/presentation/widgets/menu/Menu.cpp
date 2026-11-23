#include "presentation/widgets/menu/Menu.hpp"

Menu::Menu(std::unique_ptr<Widget> content)
    : Widget(content ? ::children(std::move(content)) : std::vector<std::unique_ptr<Widget>>{})
{
	updateSelection();
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

std::vector<MenuButton *> Menu::getButtons() const
{
	std::vector<MenuButton *> result;

	const auto collect = [&](const auto &self, Widget &widget) {
		if (auto *button = dynamic_cast<MenuButton *>(&widget)) {
			result.push_back(button);
			return;
		}

		for (const auto &child : widget.getChildren()) {
			self(self, *child);
		}
	};

	for (const auto &child : children) {
		collect(collect, *child);
	}
	return result;
}

EventResult Menu::handleEvent(const sf::Event &event)
{
	const auto *key = event.getIf<sf::Event::KeyPressed>();

	if (!key) {
		return {};
	}
	const auto buttons = getButtons();

	if (buttons.empty()) {
		return {};
	}

	switch (key->code) {
	case sf::Keyboard::Key::Down:
	case sf::Keyboard::Key::S: {
		selectedIndex = (selectedIndex + 1) % buttons.size();
		updateSelection();
		return {.handled = true};
	}
	case sf::Keyboard::Key::Up:
	case sf::Keyboard::Key::W: {
		selectedIndex = (selectedIndex + buttons.size() - 1) % buttons.size();
		updateSelection();
		return {.handled = true};
	}
	case sf::Keyboard::Key::Enter: {
		if (selectedIndex >= buttons.size()) {
			return {};
		}
		auto action = buttons.at(selectedIndex)->getCallback();
		return {.handled = true, .action = std::move(action)};
	}
	default:
		return {};
	}
}

void Menu::updateSelection() const
{
	const auto buttons = getButtons();
	for (std::size_t i = 0; i < buttons.size(); i++) {
		buttons.at(i)->setSelected(i == selectedIndex);
	}
}