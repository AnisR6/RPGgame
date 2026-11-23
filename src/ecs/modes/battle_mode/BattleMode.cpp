#include "BattleMode.hpp"

#include "ecs/prefabs/characters/enemy/Enemy.hpp"
#include "ecs/prefabs/characters/party_member/PartyMember.hpp"
#include "ecs/prefabs/characters/player/Player.hpp"
#include "ecs/prefabs/combat_action/BaseAttack.hpp"
#include "ecs/prefabs/combat_action/CombatSkill.hpp"
#include "ecs/prefabs/combat_action/Flee.hpp"
#include "ecs/prefabs/combat_action/Spell.hpp"
#include "ecs/system/combat/enemy_ai_system/EnemyAISystem.hpp"
#include "ecs/system/combat/turn_execution_system/TurnExecutionSystem.hpp"
#include "ecs/system/combat/turn_order/TurnOrderUtils.hpp"

#include <utility>

namespace Engine::ECS {
BattleMode::BattleMode(BattleMessageBus &messageBus, World &world, BattleState battleState,
                       const CombatActionExecutor executor)
    : battleModeMessageBus(messageBus), world(world), battleState(std::move(battleState)), executor(executor)
{
	subscribeToEvents();
	registerSystems();
}

void BattleMode::subscribeToEvents()
{
	battleModeMessageBus.subscribe<BattleInfoRequest>(
	    [this](const BattleInfoRequest &request) { handleBattleInfoRequest(); });
	battleModeMessageBus.subscribe<CombatActionRequest>(
	    [this](const CombatActionRequest &request) { handleCombatActionRequest(request); });
}

void BattleMode::registerSystems()
{
	systemManager.RegisterSystem<EnemyAISystem>(battleState);
	systemManager.RegisterSystem<TurnExecutionSystem>(battleState, battleModeMessageBus, executor);
}

void BattleMode::Update(const float deltaTime)
{
	world.UpdateSystems(systemManager, deltaTime);
}

void BattleMode::createParties(std::vector<Entity> &partyMembers, std::vector<Entity> &enemies) const
{
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

	auto fireball = Prefabs::createSpell(
	    world, {{},
	            {},
	            "Fireball",
	            TargetScope::SINGLE_ENEMY,
	            {{DamageEffect{.power = 10, .element = ElementType::FIRE, .magical = true}, CostMPEffect{5}}}});

	auto heal =
	    Prefabs::createSpell(world, {{}, {}, "Heal", TargetScope::SINGLE_ALLY, {{HealingEffect{10}, CostMPEffect{5}}}});

	auto iceSlash = Prefabs::createCombatSkill(
	    world, {{},
	            {},
	            "Ice Slash",
	            TargetScope::ALL_ENEMIES,
	            {{DamageEffect{.power = 4, .element = ElementType::WATER, .magical = false}}}});

	auto baseAttack = Prefabs::createBaseAttack(
	    world, {{},
	            {},
	            "Base Attack",
	            TargetScope::SINGLE_ENEMY,
	            {{DamageEffect{.power = 10, .element = ElementType::NEUTRAL, .magical = false}}}});

	auto flee = Prefabs::createFlee(world, {{}, {}, "Flee"});

	Prefabs::PlayerBlueprint playerBlueprint{
	    "John Doe", {}, {}, {}, {}, {}, defaultStats, {{baseAttack, fireball, iceSlash, heal, flee}}};

	// creating party members
	Prefabs::PartyMemberBlueprint partyMember1{"Foo", {}, {}, {{baseAttack, iceSlash, heal, flee}}};
	Prefabs::PartyMemberBlueprint partyMember2{"Bar", {}, defaultStats, {{baseAttack, heal, flee}}};
	Prefabs::PartyMemberBlueprint partyMember3{"Baz", {}, defaultStats, {{baseAttack, fireball, flee}}};

	partyMembers = {Prefabs::createPlayer(world, std::move(playerBlueprint)),
	                Prefabs::createPartyMember(world, std::move(partyMember1)),
	                Prefabs::createPartyMember(world, std::move(partyMember2)),
	                Prefabs::createPartyMember(world, std::move(partyMember3))};
	Prefabs::EnemyBlueprint enemy1{"Slime", {}, defaultStats, {{baseAttack, fireball}}, {}, {}};

	Prefabs::EnemyBlueprint enemy2{"Spider", {}, defaultStats, {{baseAttack, iceSlash}}, {}, {}};

	Prefabs::EnemyBlueprint enemy3{"Goblin", {}, defaultStats, {{baseAttack, heal}}, {}, {}};

	enemies = {Prefabs::createEnemy(world, std::move(enemy1)), Prefabs::createEnemy(world, std::move(enemy2)),
	           Prefabs::createEnemy(world, std::move(enemy3))};
}
void BattleMode::startBattle()
{
	// party members and enemies hardcoded => should be received from the outside
	std::vector<Entity> playerPartyMembers;
	std::vector<Entity> enemies;
	createParties(playerPartyMembers, enemies);

	battleState = {
	    .battlePhaseState = {BattlePhase::PlayerTurn},
	    .turnOrderState = {.playerPartyEntities = std::move(playerPartyMembers), .enemyEntities = std::move(enemies)},
	    .roundActionsState = {}};
}

// TODO: Ending Battle
void BattleMode::endBattle() const {}

void BattleMode::handleBattleInfoRequest() const
{
	const auto playerPartyEntities = battleState.turnOrderState.playerPartyEntities;
	const auto enemyPartyEntities = battleState.turnOrderState.enemyEntities;
	const auto currentActorIndex = battleState.turnOrderState.currentActorIndex;
	BattleInfoResponse response{.playerPartyEntities = playerPartyEntities,
	                            .enemyPartyEntities = enemyPartyEntities,
	                            .currentActorIndex = currentActorIndex};
	battleModeMessageBus.publish(response);
}

void BattleMode::handleCombatActionRequest(const CombatActionRequest &combatActionRequest)
{
	if (battleState.battlePhaseState.currentPhase != BattlePhase::PlayerTurn) {
		return;
	}

	auto &turnOrder = battleState.turnOrderState;

	if (turnOrder.currentActorIndex >= turnOrder.playerPartyEntities.size()) {
		return;
	}
	const Entity currentActor = turnOrder.playerPartyEntities.at(turnOrder.currentActorIndex);

	if (currentActor != combatActionRequest.actorEntity) {
		return;
	}

	const CombatAction action = BattleMapper::mapToCombatAction(combatActionRequest);

	battleState.roundActionsState.roundActions.push_back(action);

	turnOrder.currentActorIndex = TurnOrderUtils::findNextLivingPartyMemberIndex(
	    battleState.turnOrderState.playerPartyEntities, turnOrder.currentActorIndex + 1, world);

	if (turnOrder.currentActorIndex < turnOrder.playerPartyEntities.size()) {
		handleBattleInfoRequest();
		return;
	}

	battleState.battlePhaseState.currentPhase = BattlePhase::EnemyAITurn;
}

} // namespace Engine::ECS