#include "presentation/overworld/menus/InGameBootMenu.hpp"

#include "presentation/shared/widgets/menu/Menu.hpp"
#include "presentation/shared/widgets/menu/MenuButton.hpp"
#include "presentation/shared/widgets/menu/MenuColumn.hpp"
#include "presentation/shared/widgets/menu/MenuContainer.hpp"
#include "presentation/shared/widgets/menu/MenuText.hpp"

#include <utility>

std::unique_ptr<Widget> CreateInGameBootMenu(InGameBootMenuCallbacks callbacks) {
    return padding(32.f,
                   padding(16.f,
                           menu(column(children(text("Menu", 36, sf::Color::White),
                                                button(onPressed(std::move(callbacks.onResume)),
                                                       text("Resume", 28, sf::Color::White)),
                                                button(onPressed(std::move(callbacks.onExit)),
                                                       text("Exit", 28, sf::Color::White))))),
                           {.background = sf::Color(0, 0, 0, 200),
                            .border = sf::Color::White,
                            .borderThickness = 2.f}));
}
