#include "application/combat/controllers/BattleController.hpp"

#include <utility>

#include "application/combat/messages/BattleMessage.hpp"
#include "ecs/shared/components/NameComponent.hpp"
#include "ecs/combat/components/BaseAttackComponent.hpp"
#include "ecs/combat/components/CombatSkillComponent.hpp"
#include "ecs/combat/components/DeadComponent.hpp"
#include "ecs/combat/components/FleeComponent.hpp"
#include "ecs/combat/components/KnownActionsComponent.hpp"
#include "ecs/combat/components/SpellComponent.hpp"
#include "ecs/combat/components/StatsComponent.hpp"
#include "presentation/combat/screens/BattleScreen.hpp"

BattleController::BattleController(BattleMessageBus& messageBus, BattleScreen& screen,
                                   Engine::ECS::World& world)
    : messageBus(messageBus),
      screen(screen),
      world(world) {
    messageBus.Subscribe<BattleInfoResponse>(
        [this](const BattleInfoResponse& response) { UpdateBattleInfo(response); });

    messageBus.Subscribe<ShowBattleDialogRequest>(
        [this](const ShowBattleDialogRequest& request) { ShowBattleDialog(request); });

    messageBus.Subscribe<PlayBattleAnimationRequest>(
        [this](const PlayBattleAnimationRequest& request) { PlayBattleAnimation(request); });
    messageBus.Subscribe<PlayerTurnStarted>(
        [this](const PlayerTurnStarted&) { RequestBattleInfo(); });
}

void BattleController::UpdateBattleInfo(const BattleInfoResponse& response) {
    partyMembers = response.playerPartyEntities;
    enemies = response.enemyPartyEntities;
    currentActorIndex = response.currentActorIndex;

    SetBattleInfoForBattleScreen();
    SetCallbacksForBattleScreen();
    SetBattleActorMenuDataForBattleScreen();
}

void BattleController::RequestBattleInfo() const {
    messageBus.Publish(BattleInfoRequest{});
}

BattleActionMenuEntry
BattleController::CreateActionEntry(const Engine::ECS::Entity actionEntity) const {
    if (!world.HasComponent<TargetScopeComponent>(actionEntity)) {
        throw std::runtime_error("BattleController: Action has no TargetScopeComponent");
    }
    auto [name] = world.GetComponent<NameComponent>(actionEntity);
    auto [targetScope] = world.GetComponent<TargetScopeComponent>(actionEntity);
    return {.entity = actionEntity, .name = name, .targetScope = targetScope};
}

CombatantMenuEntry
BattleController::CreateCombatantMenuEntry(const Engine::ECS::Entity combatantEntity) const {
    auto [name] = world.GetComponent<NameComponent>(combatantEntity);
    const auto& stats = world.GetComponent<StatsComponent>(combatantEntity);
    return {.entity = combatantEntity,
            .name = name,
            .maxHP = stats.maxHP,
            .currHP = stats.currHP,
            .maxMP = stats.maxMP,
            .currMP = stats.currMP,
            .isDead = world.HasComponent<DeadComponent>(combatantEntity)};
}

void BattleController::ShowBattleDialog(const ShowBattleDialogRequest& request) const {
    const auto& actorName = world.GetComponent<NameComponent>(request.actorEntity).name;
    const auto& actionName = world.GetComponent<NameComponent>(request.actionEntity).name;

    std::string message = actorName + " uses " + actionName;

    if (!request.targetEntities.empty()) {
        message += " on ";
        for (std::size_t i = 0; i < request.targetEntities.size(); ++i) {
            if (i > 0 && i != request.targetEntities.size() - 1) {
                message += ", ";
            }
            if (i > 0 && i == request.targetEntities.size() - 1) {
                message += " and ";
            }
            message += world.GetComponent<NameComponent>(request.targetEntities.at(i)).name;
        }
    }
    message += '.';

    screen.ShowBattleDialog(message, [this]() { messageBus.Publish(ShowBattleDialogFinished{}); });
}

void BattleController::PlayBattleAnimation(const PlayBattleAnimationRequest& request) const {
    messageBus.Publish(PlayBattleAnimationFinished{});
}

void BattleController::SetBattleInfoForBattleScreen() const {
    std::vector<CombatantMenuEntry> playerPartyEntries;
    std::vector<CombatantMenuEntry> enemyEntries;

    for (const auto& partyMember : partyMembers) {
        playerPartyEntries.push_back(CreateCombatantMenuEntry(partyMember));
    }

    for (const auto& enemy : enemies) {
        enemyEntries.push_back(CreateCombatantMenuEntry(enemy));
    }

    screen.SetBattleInfo(playerPartyEntries, enemyEntries, currentActorIndex);
}

void BattleController::SetCallbacksForBattleScreen() {
    callbacks.onActionSelected = [this](std::vector<Engine::ECS::Entity> targetEntities,
                                        const Engine::ECS::Entity actionEntity) {
        const auto actor = partyMembers.at(currentActorIndex);

        messageBus.Publish(CombatActionRequest{.actorEntity = actor,
                                               .targetEntities = std::move(targetEntities),
                                               .actionEntity = actionEntity});
    };

    screen.SetCallbacks(std::move(callbacks));
}

void BattleController::SetBattleActorMenuDataForBattleScreen() const {
    const auto actor = partyMembers.at(currentActorIndex);
    const auto knownActionsEntities =
        world.GetComponent<KnownActionsComponent>(actor).knownActionEntities;

    std::optional<BattleActionMenuEntry> baseAttack;
    std::optional<BattleActionMenuEntry> flee;
    std::vector<BattleActionMenuEntry> spells;
    std::vector<BattleActionMenuEntry> skills;

    for (const auto knownAction : knownActionsEntities) {
        if (world.HasComponent<BaseAttackComponent>(knownAction)) {
            baseAttack = CreateActionEntry(knownAction);
        } else if (world.HasComponent<FleeComponent>(knownAction)) {
            flee = CreateActionEntry(knownAction);
        } else if (world.HasComponent<SpellComponent>(knownAction)) {
            spells.push_back(CreateActionEntry(knownAction));
        } else if (world.HasComponent<CombatSkillComponent>(knownAction)) {
            skills.push_back(CreateActionEntry(knownAction));
        }
    }

    BattleActorMenuData data{
        .baseAttack = baseAttack,
        .flee = flee,
        .spells = spells,
        .skills = skills,
    };

    screen.SetBattleActorMenuData(std::move(data));
}
