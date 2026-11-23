#include "presentation/shared/widgets/menu/MenuButton.hpp"

MenuButton::MenuButton(std::function<void()> callback, std::unique_ptr<Widget> child)
    : Widget(::children(std::move(child))),
      callback(std::move(callback)) {}

void MenuButton::Activate() const {
    if (callback) {
        callback();
    }
}

sf::Vector2f MenuButton::GetSize() const {
    return children.front()->GetSize();
}

void MenuButton::Draw(sf::RenderTarget& target, sf::Vector2f position) {
    if (selected) {
        sf::RectangleShape highlight(GetSize());
        highlight.setPosition(position);
        highlight.setFillColor(sf::Color(80, 100, 160, 100));
        highlight.setOutlineColor(sf::Color(140, 170, 255));
        highlight.setOutlineThickness(2.f);

        target.draw(highlight);
    }
    children.front()->Draw(target, position);
}
