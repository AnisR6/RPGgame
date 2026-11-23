#include "presentation/combat/screens/BattleScreen.hpp"

#include "ecs/combat/systems/TargetResolver.hpp"
#include "ecs/combat/components/TargetScopeComponent.hpp"
#include "presentation/shared/widgets/menu/Menu.hpp"
#include "presentation/shared/widgets/menu/MenuButton.hpp"
#include "presentation/shared/widgets/menu/MenuColumn.hpp"
#include "presentation/shared/widgets/menu/MenuContainer.hpp"
#include "presentation/shared/widgets/menu/MenuText.hpp"

void BattleScreen::SetCallbacks(BattleMenuCallbacks newCallbacks) {
    callbacks = std::move(newCallbacks);
}

void BattleScreen::SetBattleInfo(std::vector<CombatantMenuEntry> newPlayerParty,
                                 std::vector<CombatantMenuEntry> newEnemies,
                                 std::size_t partyMemberIndex) {
    playerParty = std::move(newPlayerParty);
    enemies = std::move(newEnemies);
    currentPartyMemberIndex = partyMemberIndex;
}

void BattleScreen::SetBattleActorMenuData(BattleActorMenuData data) {
    battleActorMenuData = std::move(data);
    ShowMain();
}

BattleScreen::BattleScreen()
    : Screen(padding(24.f, menu(), {.background = sf::Color::Black, .fillParent = true})) {}

void BattleScreen::SelectAction(const BattleActionMenuEntry& action) {
    auto actionCallback = callbacks.onActionSelected;
    const auto actionEntity = action.entity;

    ShowTargetMenu(action.targetScope, [actionCallback = std::move(actionCallback), actionEntity](
                                           std::vector<Engine::ECS::Entity> targetEntities) {
        if (actionCallback) {
            actionCallback(std::move(targetEntities), actionEntity);
        }
    });
}

std::unique_ptr<Widget> BattleScreen::CreateMainBattleMenu() {
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
            button(onPressed([this] { SelectAction(battleActorMenuData.baseAttack.value()); }),
                   padding(8.f, text("Attack", 28, sf::Color::White))));
    }

    if (!battleActorMenuData.spells.empty()) {
        widgets.push_back(button(onPressed([this] { ShowSpellMenu(); }),
                                 padding(8.f, text("Spell", 28, sf::Color::White))));
    }

    if (!battleActorMenuData.skills.empty()) {
        widgets.push_back(button(onPressed([this] { ShowSkillMenu(); }),
                                 padding(8.f, text("Skill", 28, sf::Color::White))));
    }

    widgets.push_back(
        button(onPressed([] {}), padding(8.f, text("Inventory", 28, sf::Color::White))));

    if (battleActorMenuData.flee.has_value()) {
        widgets.push_back(
            button(onPressed([this] { SelectAction(battleActorMenuData.flee.value()); }),
                   padding(8.f, text("Flee", 28, sf::Color::White))));
    }

    return menu(padding(8.f, column(std::move(widgets))));
}

std::unique_ptr<Widget>
BattleScreen::CreateSubMenu(const std::vector<BattleActionMenuEntry>& entries) {
    std::vector<std::unique_ptr<Widget>> buttons;
    for (const auto& entry : entries) {
        buttons.push_back(button(onPressed([this, entry] { SelectAction(entry); }),
                                 padding(8.f, text(entry.name, 28, sf::Color::White))));
    }

    buttons.push_back(button(onPressed([this] { ShowMain(); }),
                             padding(8.f, text("Back", 28, sf::Color::White))));
    return menu(padding(8.f, column(std::move(buttons))));
}

std::unique_ptr<Widget> BattleScreen::CreateTargetMenu(
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
        TargetResolver::Resolve(targetScope, actor, allyTargets, enemyTargets);

    for (const auto& targets : targetGroups) {
        const auto label = GetTargetLabel(targetScope, targets);
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

    buttons.push_back(button(onPressed([this] { ShowMain(); }),
                             padding(8.f, text("Back", 28, sf::Color::White))));
    return menu(padding(8.f, column(std::move(buttons))));
}

std::optional<std::string>
BattleScreen::GetTargetLabel(const TargetScope scope,
                             const std::vector<Engine::ECS::Entity>& targets) const {
    switch (scope) {
    case TargetScope::Self:
        return "Self";

    case TargetScope::AllEnemies:
        return "All Enemies";

    case TargetScope::AllAllies:
        return "All Allies";

    case TargetScope::SingleEnemy:
    case TargetScope::SingleAlly: {
        if (targets.size() != 1) {
            return std::nullopt;
        }

        const auto& entries = scope == TargetScope::SingleEnemy ? enemies : playerParty;

        const auto it = std::ranges::find(entries, targets.front(), &CombatantMenuEntry::entity);

        if (it == entries.end()) {
            return std::nullopt;
        }

        return it->name;
    }
    }
    return std::nullopt;
}

void BattleScreen::ShowMain() {
    SetRootWidget(CreateMainBattleMenu());
}

void BattleScreen::ShowSpellMenu() {
    SetRootWidget(CreateSubMenu(battleActorMenuData.spells));
}

void BattleScreen::ShowSkillMenu() {
    SetRootWidget(CreateSubMenu(battleActorMenuData.skills));
}

void BattleScreen::ShowTargetMenu(
    const TargetScope targetScope,
    const std::function<void(std::vector<Engine::ECS::Entity>)>& onTargetSelected) {
    SetRootWidget(CreateTargetMenu(targetScope, onTargetSelected));
}

void BattleScreen::ShowBattleDialog(const std::string& battleDialogMessage,
                                    std::function<void()> onFinished) {
    SetRootWidget(menu(
        padding(8.f, column(children(text(battleDialogMessage, 28, sf::Color::White),

                                     button(onPressed([this, onFinished = std::move(onFinished)]() {
                                                if (const auto callback = onFinished) {
                                                    callback();
                                                } else {
                                                    ShowMain();
                                                }
                                            }),
                                            padding(8.f, text("Continue", 20, sf::Color::White)))

                                         )))));
}
