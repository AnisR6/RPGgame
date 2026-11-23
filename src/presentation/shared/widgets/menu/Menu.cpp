#include "presentation/shared/widgets/menu/Menu.hpp"

Menu::Menu(std::unique_ptr<Widget> content)
    : Widget(content ? ::children(std::move(content)) : std::vector<std::unique_ptr<Widget>>{}) {
    UpdateSelection();
}

sf::Vector2f Menu::GetSize() const {
    if (children.empty()) {
        return {};
    }

    return children.front()->GetSize();
}

void Menu::Draw(sf::RenderTarget& target, sf::Vector2f position) {
    if (children.empty()) {
        return;
    }

    children.front()->Draw(target, position);
}

std::vector<MenuButton*> Menu::GetButtons() const {
    std::vector<MenuButton*> result;

    const auto collect = [&](const auto& self, Widget& widget) {
        if (auto* button = dynamic_cast<MenuButton*>(&widget)) {
            result.push_back(button);
            return;
        }

        for (const auto& child : widget.GetChildren()) {
            self(self, *child);
        }
    };

    for (const auto& child : children) {
        collect(collect, *child);
    }
    return result;
}

EventResult Menu::HandleEvent(const sf::Event& event) {
    const auto* key = event.getIf<sf::Event::KeyPressed>();

    if (!key) {
        return {};
    }

    switch (key->code) {
    case sf::Keyboard::Key::Down:
    case sf::Keyboard::Key::Right:
    case sf::Keyboard::Key::S:
    case sf::Keyboard::Key::D:
        return MoveSelection(1);
    case sf::Keyboard::Key::Up:
    case sf::Keyboard::Key::Left:
    case sf::Keyboard::Key::W:
    case sf::Keyboard::Key::A:
        return MoveSelection(-1);
    case sf::Keyboard::Key::Enter:
    case sf::Keyboard::Key::Space:
        return Confirm();
    default:
        return {};
    }
}

EventResult Menu::MoveSelection(int step) {
    const auto buttonCount = static_cast<int>(GetButtons().size());

    if (buttonCount == 0) {
        return {};
    }

    const int nextIndex = (static_cast<int>(selectedIndex) + step + buttonCount) % buttonCount;
    selectedIndex = static_cast<std::size_t>(nextIndex);
    UpdateSelection();

    return {.handled = true};
}

EventResult Menu::Confirm() const {
    const auto buttons = GetButtons();

    if (selectedIndex >= buttons.size()) {
        return {};
    }

    return {.handled = true, .action = buttons.at(selectedIndex)->GetCallback()};
}

void Menu::UpdateSelection() const {
    const auto buttons = GetButtons();
    for (std::size_t i = 0; i < buttons.size(); i++) {
        buttons.at(i)->SetSelected(i == selectedIndex);
    }
}
