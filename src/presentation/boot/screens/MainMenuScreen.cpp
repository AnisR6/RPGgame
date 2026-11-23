#include "presentation/boot/screens/MainMenuScreen.hpp"

#include "presentation/shared/widgets/menu/Menu.hpp"
#include "presentation/shared/widgets/menu/MenuButton.hpp"
#include "presentation/shared/widgets/menu/MenuColumn.hpp"
#include "presentation/shared/widgets/menu/MenuContainer.hpp"
#include "presentation/shared/widgets/menu/MenuText.hpp"

#include <utility>

MainMenuScreen::MainMenuScreen(BootMenuCallbacks callbacks)
    : Screen(padding(24.f,
                     menu(column(children(text("My RPG", 48, sf::Color::White),
                                          button(onPressed(std::move(callbacks.onStartGame)),
                                                 text("Start Game", 28, sf::Color::White)),
                                          button(onPressed(std::move(callbacks.onLoadGame)),
                                                 text("Load Game", 28, sf::Color::White)),
                                          button(onPressed(std::move(callbacks.onExit)),
                                                 text("Exit Game", 28, sf::Color::White))))),
                     {.background = sf::Color::Black, .fillParent = true})) {}
