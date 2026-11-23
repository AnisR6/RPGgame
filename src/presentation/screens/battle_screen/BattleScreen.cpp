#include "BattleScreen.hpp"

#include "../../../ecs/system/combat/target_resolver/TargetResolver.hpp"
#include "ecs/components/combat/TargetScopeComponent.hpp"
#include "presentation/widgets/menu/Menu.hpp"
#include "presentation/widgets/menu/MenuButton.hpp"
#include "presentation/widgets/menu/MenuColumn.hpp"
#include "presentation/widgets/menu/MenuContainer.hpp"
#include "presentation/widgets/menu/MenuText.hpp"

BattleScreen& BattleScreen::getInstance() {
    static BattleScreen instance;
    return instance;
}

void BattleScreen::setCallbacks(BattleMenuCallbacks newCallbacks) {
    callbacks = std::move(newCallbacks);
}

void BattleScreen::setBattleInfo(std::vector<CombatantMenuEntry> newPlayerParty,
                                 std::vector<CombatantMenuEntry> newEnemies,
                                 std::size_t partyMemberIndex) {
    playerParty = std::move(newPlayerParty);
    enemies = std::move(newEnemies);
    currentPartyMemberIndex = partyMemberIndex;
}

void BattleScreen::setBattleActorMenuData(BattleActorMenuData data) {
    battleActorMenuData = std::move(data);
    showMain();
}

BattleScreen::BattleScreen()
    : Screen(padding(24.f, menu(), {.background = sf::Color::Black, .fillParent = true})) {}

void BattleScreen::selectAction(const BattleActionMenuEntry& action) {
    auto actionCallback = callbacks.onActionSelected;
    const auto actionEntity = action.entity;

    showTargetMenu(action.targetScope, [actionCallback = std::move(actionCallback), actionEntity](
                                           std::vector<Engine::ECS::Entity> targetEntities) {
        if (actionCallback) {
            actionCallback(std::move(targetEntities), actionEntity);
        }
    });
}

std::unique_ptr<Widget> BattleScreen::createMainBattleMenu() {
    std::vector<std::unique_ptr<Widget>> widgets;
    const auto& currPartyMember = playerParty.at(currentPartyMemberIndex);

    widgets.push_back(padding(8.f, text(currPartyMember.name, 28, sf::Color::White)));
    widgets.push_back(
        padding(8.f, text(std::format("HP: {}/{}", currPartyMember.currHP, currPartyMember.maxHP),
                          28, sf::Color::White)));

    widgets.push_back(
        padding(8.f, text(std::format("MP: {}/{}", currPartyMember.currMP, currPartyMember.maxMP),
                          28, sf::Color::White)));

    if (battleActorMenuData.baseAttack.has_value()) {
        widgets.push_back(
            button(onPressed([this] { selectAction(battleActorMenuData.baseAttack.value()); }),
                   padding(8.f, text("Attack", 28, sf::Color::White))));
    }

    if (!battleActorMenuData.spells.empty()) {
        widgets.push_back(button(onPressed([this] { showSpellMenu(); }),
                                 padding(8.f, text("Spell", 28, sf::Color::White))));
    }

    if (!battleActorMenuData.skills.empty()) {
        widgets.push_back(button(onPressed([this] { showSkillMenu(); }),
                                 padding(8.f, text("Skill", 28, sf::Color::White))));
    }

    widgets.push_back(
        button(onPressed([] {}), padding(8.f, text("Inventory", 28, sf::Color::White))));

    if (battleActorMenuData.flee.has_value()) {
        widgets.push_back(
            button(onPressed([this] { selectAction(battleActorMenuData.flee.value()); }),
                   padding(8.f, text("Flee", 28, sf::Color::White))));
    }

    return menu(padding(8.f, column(std::move(widgets))));
}

std::unique_ptr<Widget>
BattleScreen::createSubMenu(const std::vector<BattleActionMenuEntry>& entries) {
    std::vector<std::unique_ptr<Widget>> buttons;
    for (const auto& entry : entries) {
        buttons.push_back(button(onPressed([this, entry] { selectAction(entry); }),
                                 padding(8.f, text(entry.name, 28, sf::Color::White))));
    }

    buttons.push_back(button(onPressed([this] { showMain(); }),
                             padding(8.f, text("Back", 28, sf::Color::White))));
    return menu(padding(8.f, column(std::move(buttons))));
}

std::unique_ptr<Widget> BattleScreen::createTargetMenu(
    const TargetScope targetScope,
    const std::function<void(std::vector<Engine::ECS::Entity>)>& onTargetSelected) {
    std::vector<std::unique_ptr<Widget>> buttons;
    std::vector<Engine::ECS::Entity> allyTargets;
    std::vector<Engine::ECS::Entity> enemyTargets;

    for (const auto& entry : playerParty) {
        if (!entry.isDead) {
            allyTargets.push_back(entry.entity);
        }
    }
    for (const auto& entry : enemies) {
        if (!entry.isDead) {
            enemyTargets.push_back(entry.entity);
        }
    }

    const Engine::ECS::Entity actor = playerParty.at(currentPartyMemberIndex).entity;

    const auto targetGroups =
        Engine::ECS::TargetResolver::resolve(targetScope, actor, allyTargets, enemyTargets);

    for (const auto& targets : targetGroups) {
        const auto label = getTargetLabel(targetScope, targets);
        if (!label) {
            continue;
        }
        buttons.push_back(button(onPressed([onTargetSelected, targets] {
                                     auto callback = onTargetSelected;
                                     auto safeTargets = targets;
                                     if (callback) {
                                         callback(std::move(safeTargets));
                                     }
                                 }),
                                 padding(8.f, text(label.value(), 28, sf::Color::White))));
    }

    buttons.push_back(button(onPressed([this] { showMain(); }),
                             padding(8.f, text("Back", 28, sf::Color::White))));
    return menu(padding(8.f, column(std::move(buttons))));
}

std::optional<std::string>
BattleScreen::getTargetLabel(const TargetScope scope,
                             const std::vector<Engine::ECS::Entity>& targets) const {
    switch (scope) {
    case TargetScope::SELF:
        return "Self";

    case TargetScope::ALL_ENEMIES:
        return "All Enemies";

    case TargetScope::ALL_ALLIES:
        return "All Allies";

    case TargetScope::SINGLE_ENEMY:
    case TargetScope::SINGLE_ALLY: {
        if (targets.size() != 1) {
            return std::nullopt;
        }

        const auto& entries = scope == TargetScope::SINGLE_ENEMY ? enemies : playerParty;

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

void BattleScreen::showMain() {
    setRootWidget(createMainBattleMenu());
}

void BattleScreen::showSpellMenu() {
    setRootWidget(createSubMenu(battleActorMenuData.spells));
}

void BattleScreen::showSkillMenu() {
    setRootWidget(createSubMenu(battleActorMenuData.skills));
}

void BattleScreen::showTargetMenu(
    const TargetScope targetScope,
    const std::function<void(std::vector<Engine::ECS::Entity>)>& onTargetSelected) {
    setRootWidget(createTargetMenu(targetScope, onTargetSelected));
}

// Dialog
void BattleScreen::showBattleDialog(const std::string& battleDialogMessage,
                                    std::function<void()> onFinished) {
    setRootWidget(menu(
        padding(8.f, column(children(text(battleDialogMessage, 28, sf::Color::White),

                                     button(onPressed([this, onFinished = std::move(onFinished)]() {
                                                if (const auto callback = onFinished) {
                                                    callback();
                                                } else {
                                                    showMain();
                                                }
                                            }),
                                            padding(8.f, text("Continue", 20, sf::Color::White)))

                                         )))));
}