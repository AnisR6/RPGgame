#include "presentation/screens/boot_screen/LoadMenuScreen.hpp"

#include "presentation/widgets/menu/Menu.hpp"
#include "presentation/widgets/menu/MenuButton.hpp"
#include "presentation/widgets/menu/MenuColumn.hpp"
#include "presentation/widgets/menu/MenuContainer.hpp"
#include "presentation/widgets/menu/MenuText.hpp"

LoadMenuScreen &LoadMenuScreen::getInstance()
{
	static LoadMenuScreen instance;
	return instance;
}

LoadMenuScreen::LoadMenuScreen()
    : Screen(padding(24.f, menu(column(children(text("Load Game", 48, sf::Color::White),
                                                button(onPressed([] {}), text("Load 1", 28, sf::Color::White)),
                                                button(onPressed([] {}), text("Load 2", 28, sf::Color::White)),
                                                button(onPressed([] {}), text("Load 3", 28, sf::Color::White)),
                                                button(onPressed([] {}), text("Back", 28, sf::Color::White)))))))
{
}
