#include "BattleScreen.hpp"

#include "presentation/widgets/menu/Menu.hpp"
#include "presentation/widgets/menu/MenuButton.hpp"
#include "presentation/widgets/menu/MenuColumn.hpp"
#include "presentation/widgets/menu/MenuContainer.hpp"
#include "presentation/widgets/menu/MenuText.hpp"

BattleScreen &BattleScreen::getInstance()
{
	static BattleScreen instance;
	return instance;
}

BattleScreen::BattleScreen()
    : Screen(padding(24.f,
                     menu(column(children(text("Combat", 48, sf::Color::White),
                                          button(onPressed([] {}), text("Attack", 28, sf::Color::White)),
                                          button(onPressed([] {}), text("Spell", 28, sf::Color::White)),
                                          button(onPressed([] {}), text("Skill", 28, sf::Color::White)),
                                          button(onPressed([] {}), text("Inventory", 28, sf::Color::White)),
                                          button(onPressed([] {}), text("Flight", 28, sf::Color::White))))),
                     {.background = sf::Color::Black, .fillParent = true}))
{
}
