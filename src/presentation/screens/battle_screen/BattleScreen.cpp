#include "BattleScreen.hpp"

#include "presentation/widgets/menu/Menu.hpp"
#include "presentation/widgets/menu/MenuButton.hpp"
#include "presentation/widgets/menu/MenuColumn.hpp"
#include "presentation/widgets/menu/MenuContainer.hpp"
#include "presentation/widgets/menu/MenuText.hpp"

#include "game_logic/systems/game/battle/battle_system/Structure/CombatSkill.hpp"
#include "game_logic/systems/game/battle/battle_system/Structure/Spell.hpp"

BattleScreen &BattleScreen::getInstance()
{
	static BattleScreen instance;
	return instance;
}

BattleScreen::BattleScreen() : Screen(padding(24.f, menu(), {.background = sf::Color::Black, .fillParent = true}))
{
	// start off in main combat menu after initialisation
	showMain();
}

std::unique_ptr<Widget> BattleScreen::createMainBattleMenu()
{
	return menu(column(
	    children(text("Combat", 48, sf::Color::White), button(onPressed([] {}), text("Attack", 28, sf::Color::White)),
	             button(onPressed([this] { showSubMenu<Spell>(); }), text("Spell", 28, sf::Color::White)),
	             button(onPressed([this] { showSubMenu<CombatSkill>(); }), text("Skill", 28, sf::Color::White)),
	             button(onPressed([] {}), text("Inventory", 28, sf::Color::White)),
	             button(onPressed([] {}), text("Flight", 28, sf::Color::White)))));
}

// show functions sets the respective menu as content of the screen

void BattleScreen::showMain() {}

template <typename T>
std::unique_ptr<Widget> BattleScreen::createSubMenu(std::vector<T> &subMenuEntry)
{
}

template <typename T>
void BattleScreen::showSubMenu()
{
}
