#include "presentation/boot/screens/LoadGameScreen.hpp"

#include "presentation/shared/widgets/menu/Menu.hpp"
#include "presentation/shared/widgets/menu/MenuButton.hpp"
#include "presentation/shared/widgets/menu/MenuColumn.hpp"
#include "presentation/shared/widgets/menu/MenuContainer.hpp"
#include "presentation/shared/widgets/menu/MenuText.hpp"

#include <utility>

LoadGameScreen::LoadGameScreen(std::function<void()> onBack)
    : Screen(
          padding(24.f,
                  menu(column(children(
                      text("Load Game", 48, sf::Color::White),
                      button(onPressed([] {}), text("Slot 1 - Empty", 28, sf::Color::White)),
                      button(onPressed([] {}), text("Slot 2 - Empty", 28, sf::Color::White)),
                      button(onPressed([] {}), text("Slot 3 - Empty", 28, sf::Color::White)),
                      button(onPressed(std::move(onBack)), text("Back", 28, sf::Color::White))))),
                  {.background = sf::Color::Black, .fillParent = true})) {}
