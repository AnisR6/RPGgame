#include "presentation/screens/boot_screen/MainMenuScreen.hpp"

#include "presentation/widgets/menu/Menu.hpp"
#include "presentation/widgets/menu/MenuButton.hpp"
#include "presentation/widgets/menu/MenuColumn.hpp"
#include "presentation/widgets/menu/MenuContainer.hpp"
#include "presentation/widgets/menu/MenuText.hpp"

MainMenuScreen &MainMenuScreen::getInstance()
{
	static MainMenuScreen instance;
	return instance;
}

MainMenuScreen::MainMenuScreen()
    : Screen(padding(24.f, menu(column(children(text("My RPG", 48, sf::Color::White),
	                                            button(onPressed([] {}), text("Start Game", 28, sf::Color::White)),
	                                            button(onPressed([] {}), text("Load Game", 28, sf::Color::White)),
	                                            button(onPressed([] {}), text("Controls", 28, sf::Color::White)),
	                                            button(onPressed([] {}), text("Exit Game", 28, sf::Color::White)))),
	                            {.background = sf::Color::Black, .fillParent = true})))
{
}
