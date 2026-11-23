#pragma once

#include "presentation/shared/widgets/core/Widget.hpp"
#include "presentation/shared/widgets/menu/MenuButton.hpp"

#include <memory>
#include <utility>

class Menu : public Widget {
    private:
        std::size_t selectedIndex = 0;

        [[nodiscard]] std::vector<MenuButton*> GetButtons() const;

        EventResult MoveSelection(int step);
        EventResult Confirm() const;

    public:
        Menu() = default;

        explicit Menu(std::unique_ptr<Widget> content);

        [[nodiscard]] sf::Vector2f GetSize() const override;

        void Draw(sf::RenderTarget& target, sf::Vector2f position) override;

        EventResult HandleEvent(const sf::Event& event) override;

        void UpdateSelection() const;
};

inline std::unique_ptr<Menu> menu(std::unique_ptr<Widget> content = nullptr) {
    return std::make_unique<Menu>(std::move(content));
}
