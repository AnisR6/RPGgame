#include "application/combat/modes/BattleMode.hpp"

#include "application/combat/mappers/BattleMapper.hpp"
#include "ecs/combat/prefabs/Enemy.hpp"
#include "ecs/combat/prefabs/PartyMember.hpp"
#include "ecs/shared/prefabs/Player.hpp"
#include "ecs/combat/prefabs/BaseAttack.hpp"
#include "ecs/combat/prefabs/CombatSkill.hpp"
#include "ecs/combat/prefabs/Flee.hpp"
#include "ecs/combat/prefabs/Spell.hpp"
#include "ecs/combat/systems/EnemyAISystem.hpp"
#include "ecs/combat/systems/TurnExecutionSystem.hpp"
#include "ecs/combat/systems/TurnOrder.hpp"
#include "presentation/shared/rendering/Renderer.hpp"

#include <utility>
using Engine::ECS::Entity;

BattleMode::BattleMode(Engine::ECS::World& world, Renderer& renderer)
    : world(world),
      renderer(renderer),
      executor(world),
      controller(messageBus, screen, world) {
    SubscribeToEvents();
    RegisterSystems();
}

void BattleMode::OnEnter() {
    StartBattle();
    controller.RequestBattleInfo();
}

void BattleMode::OnExit() {
    EndBattle();
}

bool BattleMode::HandleEvent(const sf::Event& event) {
    return screen.HandleEvent(event);
}

void BattleMode::Render(sf::RenderWindow& window) {
    renderer.Render(window, screen);
}

void BattleMode::SubscribeToEvents() {
    messageBus.Subscribe<BattleInfoRequest>(
        [this](const BattleInfoRequest&) { HandleBattleInfoRequest(); });
    messageBus.Subscribe<CombatActionRequest>(
        [this](const CombatActionRequest& request) { HandleCombatActionRequest(request); });
}

void BattleMode::RegisterSystems() {
    systemManager.RegisterSystem<EnemyAISystem>(battleState);
    systemManager.RegisterSystem<TurnExecutionSystem>(battleState, messageBus, executor);
}

void BattleMode::Update(const float deltaTime) {
    world.UpdateSystems(systemManager, deltaTime);
}

void BattleMode::CreateParties(std::vector<Entity>& partyMembers,
                               std::vector<Entity>& enemies) const {
    StatsComponent defaultStats{.maxHP = 100,
                                .maxMP = 100,
                                .currHP = 100,
                                .currMP = 100,
                                .attack = 10,
                                .defense = 10,
                                .magic = 10,
                                .resistance = 10,
                                .skill = 10,
                                .speed = 100};

    auto fireball = Prefabs::CreateSpell(
        world, {{},
                {},
                "Fireball",
                TargetScope::SingleEnemy,
                {{DamageEffect{.power = 10, .element = ElementType::Fire, .magical = true},
                  CostMPEffect{5}}}});

    auto heal = Prefabs::CreateSpell(
        world, {{}, {}, "Heal", TargetScope::SingleAlly, {{HealingEffect{10}, CostMPEffect{5}}}});

    auto iceSlash = Prefabs::CreateCombatSkill(
        world, {{},
                {},
                "Ice Slash",
                TargetScope::AllEnemies,
                {{DamageEffect{.power = 4, .element = ElementType::Water, .magical = false}}}});

    auto baseAttack = Prefabs::CreateBaseAttack(
        world, {{},
                {},
                "Base Attack",
                TargetScope::SingleEnemy,
                {{DamageEffect{.power = 10, .element = ElementType::Neutral, .magical = false}}}});

    auto flee = Prefabs::CreateFlee(world, {{}, {}, "Flee"});

    Prefabs::PlayerBlueprint playerBlueprint{
        "John Doe", {}, {},           {},
        {},         {}, defaultStats, {{baseAttack, fireball, iceSlash, heal, flee}}};

    Prefabs::PartyMemberBlueprint partyMember1{"Foo", {}, {}, {{baseAttack, iceSlash, heal, flee}}};
    Prefabs::PartyMemberBlueprint partyMember2{"Bar", {}, defaultStats, {{baseAttack, heal, flee}}};
    Prefabs::PartyMemberBlueprint partyMember3{
        "Baz", {}, defaultStats, {{baseAttack, fireball, flee}}};

    partyMembers = {Prefabs::CreatePlayer(world, std::move(playerBlueprint)),
                    Prefabs::CreatePartyMember(world, std::move(partyMember1)),
                    Prefabs::CreatePartyMember(world, std::move(partyMember2)),
                    Prefabs::CreatePartyMember(world, std::move(partyMember3))};

    Prefabs::EnemyBlueprint enemy1{"Slime", {}, defaultStats, {{baseAttack, fireball}}, {}, {}};
    Prefabs::EnemyBlueprint enemy2{"Spider", {}, defaultStats, {{baseAttack, iceSlash}}, {}, {}};
    Prefabs::EnemyBlueprint enemy3{"Goblin", {}, defaultStats, {{baseAttack, heal}}, {}, {}};

    enemies = {Prefabs::CreateEnemy(world, std::move(enemy1)),
               Prefabs::CreateEnemy(world, std::move(enemy2)),
               Prefabs::CreateEnemy(world, std::move(enemy3))};
}

void BattleMode::StartBattle() {
    std::vector<Entity> playerPartyMembers;
    std::vector<Entity> enemies;
    CreateParties(playerPartyMembers, enemies);

    battleState = {.battlePhaseState = {BattlePhase::PlayerTurn},
                   .turnOrderState = {.playerPartyEntities = std::move(playerPartyMembers),
                                      .enemyEntities = std::move(enemies)},
                   .roundActionsState = {}};
}

void BattleMode::EndBattle() const {}

void BattleMode::HandleBattleInfoRequest() const {
    const auto& turnOrder = battleState.turnOrderState;

    messageBus.Publish(BattleInfoResponse{.playerPartyEntities = turnOrder.playerPartyEntities,
                                          .enemyPartyEntities = turnOrder.enemyEntities,
                                          .currentActorIndex = turnOrder.currentActorIndex});
}

void BattleMode::HandleCombatActionRequest(const CombatActionRequest& combatActionRequest) {
    if (battleState.battlePhaseState.currentPhase != BattlePhase::PlayerTurn) {
        return;
    }

    auto& turnOrder = battleState.turnOrderState;

    if (turnOrder.currentActorIndex >= turnOrder.playerPartyEntities.size()) {
        return;
    }

    const Entity currentActor = turnOrder.playerPartyEntities.at(turnOrder.currentActorIndex);

    if (currentActor != combatActionRequest.actorEntity) {
        return;
    }

    battleState.roundActionsState.roundActions.push_back(
        BattleMapper::MapToCombatAction(combatActionRequest));

    turnOrder.currentActorIndex = TurnOrder::FindNextLivingPartyMemberIndex(
        turnOrder.playerPartyEntities, turnOrder.currentActorIndex + 1, world);

    if (turnOrder.currentActorIndex < turnOrder.playerPartyEntities.size()) {
        HandleBattleInfoRequest();
        return;
    }

    battleState.battlePhaseState.currentPhase = BattlePhase::EnemyAITurn;
}
