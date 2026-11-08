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
                                 const std::vector<CombatantInfo> &enemies, const std::size_t partyMemberIndex)
{
	playerPartyInfo = playerParty;
	enemyPartyInfo = enemies;
	currentPartyMemberIndex = partyMemberIndex;
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
	                    // TODO: base attack target scope should depend on equipped weapon
	                    button(onPressed([this] {
		                           showTargetMenu(TargetScope::SINGLE_ENEMY, [this](std::vector<int> targetIds) {
			                           callbacks.onBaseAttack(std::move(targetIds));
		                           });
	                           }),
	                           text("Attack", 28, sf::Color::White)),
	                    button(onPressed([this] { showSubMenu<Spell>(); }), text("Spell", 28, sf::Color::White)),
	                    button(onPressed([this] { showSubMenu<CombatSkill>(); }), text("Skill", 28, sf::Color::White)),
	                    button(onPressed([] {}), text("Inventory", 28, sf::Color::White)),
	                    button(onPressed(callbacks.onFlight), text("Flight", 28, sf::Color::White)))));
}

template <typename T, typename GetId, typename GetName, typename GetTargetScope>
std::unique_ptr<Widget> BattleScreen::createSubMenu(const std::vector<T> &subMenuEntries,
                                                    const std::function<void(std::vector<int>, int)> &onSelected,
                                                    GetId getId, GetName getName, GetTargetScope getTargetScope)
{
	std::vector<std::unique_ptr<Widget>> buttons;

	for (const auto &entry : subMenuEntries) {
		buttons.push_back(button(onPressed([id = getId(entry), targetScope = getTargetScope(entry), onSelected, this] {
			                         showTargetMenu(targetScope, [id, onSelected](std::vector<int> targets) {
				                         onSelected(std::move(targets), id);
			                         });
		                         }),
		                         text(getName(entry), 22, sf::Color::White)));
	}

	buttons.push_back(button(onPressed([this] { showMain(); }), text("Back", 22, sf::Color::White)));

	return menu(column(std::move(buttons)));
}

std::unique_ptr<Widget> BattleScreen::createTargetMenu(const TargetScope targetScope,
                                                       const std::function<void(std::vector<int>)> &onTargetSelected)
{
	std::vector<std::unique_ptr<Widget>> buttons;
	std::vector<CombatantInfo> possibleTargets;

	switch (targetScope) {
	case TargetScope::SINGLE_ENEMY:
		possibleTargets = enemyPartyInfo;
		break;
	case TargetScope::ALL_ENEMIES: {
		std::vector<int> enemyIds;
		for (const auto &enemy : enemyPartyInfo) {
			enemyIds.push_back(enemy.id);
		}
		buttons.push_back(button(onPressed([onTargetSelected, enemyIds] { onTargetSelected(enemyIds); }),
		                         text("All Enemies", 22, sf::Color::White)));
		break;
	}
	case TargetScope::SINGLE_ALLIE:
		possibleTargets = playerPartyInfo;
		break;
	case TargetScope::ALL_ALLIES: {
		std::vector<int> partyIds;
		for (const auto &partyMember : playerPartyInfo) {
			partyIds.push_back(partyMember.id);
		}
		buttons.push_back(button(onPressed([onTargetSelected, partyIds] { onTargetSelected(partyIds); }),
		                         text("All Allies", 22, sf::Color::White)));
		break;
	}
	case TargetScope::SELF: {
		buttons.push_back(button(
		    onPressed([onTargetSelected, this] { onTargetSelected({playerPartyInfo.at(currentPartyMemberIndex).id}); }),
		    text("Self", 22, sf::Color::White)));
		break;
	}
	}

	if (targetScope == TargetScope::SINGLE_ENEMY || targetScope == TargetScope::SINGLE_ALLIE) {
		for (const auto &posTarget : possibleTargets) {
			buttons.push_back(button(onPressed([onTargetSelected, posTarget] { onTargetSelected({posTarget.id}); }),
			                         text(posTarget.name, 22, sf::Color::White)));
		}
	}

	buttons.push_back(button(onPressed([this] { showMain(); }), text("Back", 22, sf::Color::White)));
	return menu(column(std::move(buttons)));
}

// show functions sets the respective menu as content of the screen

void BattleScreen::showMain()
{
	setRootWidget(createMainBattleMenu());
}

template <typename T>
void BattleScreen::showSubMenu()
{
	const auto &currentPartyMemberInfo = playerPartyInfo.at(currentPartyMemberIndex);
	// createSubMenu does not get set yet
	if constexpr (std::is_same_v<T, Spell>) {
		setRootWidget(createSubMenu(
		    currentPartyMemberInfo.spells, callbacks.onSpell, [](const Spell &s) { return s.spellId; },
		    [](const Spell &s) { return s.spellName; }, [](const Spell &s) { return s.targetScope; }));

	} else if constexpr (std::is_same_v<T, CombatSkill>) {
		setRootWidget(createSubMenu(
		    currentPartyMemberInfo.skills, callbacks.onSkill, [](const CombatSkill &s) { return s.skillId; },
		    [](const CombatSkill &s) { return s.skillName; }, [](const CombatSkill &s) { return s.targetScope; }));
	}
}

void BattleScreen::showTargetMenu(const TargetScope targetScope,
                                  const std::function<void(std::vector<int>)> &onTargetSelected)
{
	setRootWidget(createTargetMenu(targetScope, onTargetSelected));
}