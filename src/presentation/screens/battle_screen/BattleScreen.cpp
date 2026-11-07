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

void BattleScreen::setCallbacks(BattleMenuCallbacks newCallbacks)
{
	callbacks = std::move(newCallbacks);
}

void BattleScreen::setBattleInfo(const std::vector<CombatantInfo> &playerParty,
                                 const std::vector<CombatantInfo> &enemies, std::size_t partyMemberIndex)
{
	playerPartyInfo = playerParty;
	enemyPartyInfo = enemies;
	this->currentPartyMemberIndex = partyMemberIndex;
}

BattleScreen::BattleScreen() : Screen(padding(24.f, menu(), {.background = sf::Color::Black, .fillParent = true}))
{
	// start off in main combat menu after initialisation
	showMain();
}

std::unique_ptr<Widget> BattleScreen::createMainBattleMenu()
{
	return menu(
	    column(children(text("Combat", 48, sf::Color::White),
	                    button(onPressed(callbacks.onBaseAttack), text("Attack", 28, sf::Color::White)),
	                    button(onPressed([this] { showSubMenu<Spell>(); }), text("Spell", 28, sf::Color::White)),
	                    button(onPressed([this] { showSubMenu<CombatSkill>(); }), text("Skill", 28, sf::Color::White)),
	                    button(onPressed([] {}), text("Inventory", 28, sf::Color::White)),
	                    button(onPressed(callbacks.onFlight), text("Flight", 28, sf::Color::White)))));
}

// show functions sets the respective menu as content of the screen

void BattleScreen::showMain() {}

template <typename T, typename GetId, typename GetName>
std::unique_ptr<Widget> BattleScreen::createSubMenu(const std::vector<T> &subMenuEntries,
                                                    const std::function<void(int)> &onSelected, GetId getId,
                                                    GetName getName)
{
	std::vector<std::unique_ptr<Widget>> buttons;

	for (const auto &entry : subMenuEntries) {
		buttons.push_back(button(onPressed([id = getId(entry), onSelected] { onSelected(id); }),
		                         text(getName(entry), 22, sf::Color::White)));
	}

	buttons.push_back(button(onPressed([this] { showMain(); }), text("Back", 22, sf::Color::White)));

	return menu(column(std::move(buttons)));
}

template <typename T>
void BattleScreen::showSubMenu()
{
	const auto &currentPartyMemberInfo = playerPartyInfo.at(currentPartyMemberIndex);
	// createSubMenu does not get set yet
	if constexpr (std::is_same_v<T, Spell>) {
		const auto &spells = playerPartyInfo.at(currentPartyMemberIndex).spells;
		createSubMenu(
		    spells, callbacks.onSpell, [](const Spell &s) { return s.spellId; },
		    [](const Spell &s) { return s.spellName; });

	} else if constexpr (std::is_same_v<T, CombatSkill>) {
		createSubMenu(
		    currentPartyMemberInfo.skills, callbacks.onSkill, [](const CombatSkill &s) { return s.skillId; },
		    [](const CombatSkill &s) { return s.skillName; });
	}
}
