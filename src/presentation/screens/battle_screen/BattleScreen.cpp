#include "BattleScreen.hpp"

#include "../../../game_state/ecs/system/combat/target_resolver/TargetResolver.hpp"
#include "game_state/ecs/components/combat/TargetScopeComponent.hpp"
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

void BattleScreen::setCallbacks(BattleMenuCallbacks newCallbacks)
{
	callbacks = std::move(newCallbacks);
}

void BattleScreen::setBattleInfo(std::vector<CombatantMenuEntry> newPlayerParty,
                                 std::vector<CombatantMenuEntry> newEnemies, std::size_t partyMemberIndex)
{
	playerParty = std::move(newPlayerParty);
	enemies = std::move(newEnemies);
	currentPartyMemberIndex = partyMemberIndex;
}

void BattleScreen::setBattleActorMenuData(BattleActorMenuData data)
{
	battleActorMenuData = std::move(data);
	showMain();
}

BattleScreen::BattleScreen() : Screen(padding(24.f, menu(), {.background = sf::Color::Black, .fillParent = true}))
{
	showMain();
}

void BattleScreen::selectAction(const BattleActionMenuEntry &action)
{
	showTargetMenu(action.targetScope,
	               [this, actionEntity = action.entity](std::vector<Engine::ECS::Entity> targetEntities) {
		               callbacks.onActionSelected(std::move(targetEntities), actionEntity);
	               });
}

std::unique_ptr<Widget> BattleScreen::createMainBattleMenu()
{
	return menu(column(children(text("Combat", 48, sf::Color::White),
	                            button(onPressed([this] { selectAction(battleActorMenuData.baseAttack); }),
	                                   text("Attack", 28, sf::Color::White)),
	                            button(onPressed([this] { showSpellMenu(); }), text("Spell", 28, sf::Color::White)),
	                            button(onPressed([this] { showSkillMenu(); }), text("Skill", 28, sf::Color::White)),
	                            button(onPressed([] {}), text("Inventory", 28, sf::Color::White)),
	                            button(onPressed([this] { selectAction(battleActorMenuData.flight); }),
	                                   text("Flight", 28, sf::Color::White)))));
}

std::unique_ptr<Widget> BattleScreen::createSubMenu(const std::vector<BattleActionMenuEntry> &entries)
{
	std::vector<std::unique_ptr<Widget>> buttons;
	for (const auto &entry : entries) {
		buttons.push_back(
		    button(onPressed([this, entry] {
			           showTargetMenu(entry.targetScope,
			                          [this, action = entry.entity](std::vector<Engine::ECS::Entity> targetEntities) {
				                          callbacks.onActionSelected(std::move(targetEntities), action);
			                          });
		           }),
		           text(entry.name, 22, sf::Color::White)));
	}

	buttons.push_back(button(onPressed([this] { showMain(); }), text("Back", 22, sf::Color::White)));
	return menu(column(std::move(buttons)));
}

std::unique_ptr<Widget>
BattleScreen::createTargetMenu(const TargetScope targetScope,
                               const std::function<void(std::vector<Engine::ECS::Entity>)> &onTargetSelected)
{
	std::vector<std::unique_ptr<Widget>> buttons;
	std::vector<Engine::ECS::Entity> allyTargets;
	std::vector<Engine::ECS::Entity> enemyTargets;

	for (const auto &entry : playerParty) {
		allyTargets.push_back(entry.entity);
	}
	for (const auto &entry : enemies) {
		enemyTargets.push_back(entry.entity);
	}

	const Engine::ECS::Entity actor = playerParty.at(currentPartyMemberIndex).entity;

	const auto targetGroups = Engine::ECS::TargetResolver::resolve(targetScope, actor, allyTargets, enemyTargets);

	for (const auto &targets : targetGroups) {
		const auto label = getTargetLabel(targetScope, targets);
		if (!label) {
			continue;
		}
		buttons.push_back(button(onPressed([onTargetSelected, targets] { onTargetSelected(targets); }),
		                         text(label.value(), 22, sf::Color::White)));
	}

	buttons.push_back(button(onPressed([this] { showMain(); }), text("Back", 22, sf::Color::White)));
	return menu(column(std::move(buttons)));
}

std::optional<std::string> BattleScreen::getTargetLabel(const TargetScope scope,
                                                        const std::vector<Engine::ECS::Entity> &targets) const
{
	switch (scope) {
	case TargetScope::SELF:
		return "Self";

	case TargetScope::ALL_ENEMIES:
		return "All Enemies";

	case TargetScope::ALL_ALLIES:
		return "All Allies";

	case TargetScope::SINGLE_ENEMY:
	case TargetScope::SINGLE_ALLIE: {
		if (targets.size() != 1) {
			return std::nullopt;
		}

		const auto &entries = scope == TargetScope::SINGLE_ENEMY ? enemies : playerParty;

		const auto it = std::ranges::find(entries, targets.front(), &CombatantMenuEntry::entity);

		if (it == entries.end()) {
			return std::nullopt;
		}

		return it->name;
	}
	}
	return std::nullopt;
}

// show functions sets the respective menu as content of the screen

void BattleScreen::showMain()
{
	setRootWidget(createMainBattleMenu());
}

void BattleScreen::showSpellMenu()
{
	setRootWidget(createSubMenu(battleActorMenuData.spells));
}

void BattleScreen::showSkillMenu()
{
	setRootWidget(createSubMenu(battleActorMenuData.skills));
}

void BattleScreen::showTargetMenu(const TargetScope targetScope,
                                  const std::function<void(std::vector<Engine::ECS::Entity>)> &onTargetSelected)
{
	setRootWidget(createTargetMenu(targetScope, onTargetSelected));
}