#include "BattleMode.hpp"

#include "game_state/ecs/prefabs/characters/enemy/Enemy.hpp"
#include "game_state/ecs/prefabs/characters/party_member/PartyMember.hpp"
#include "game_state/ecs/prefabs/characters/player/Player.hpp"
#include "game_state/ecs/prefabs/combat/BattleState.hpp"

#include <iostream>

BattleMode::BattleMode(BattleMessageBus &messageBus, Engine::ECS::World &world)
    : battleModeMessageBus(messageBus), world(world)
{
	messageBus.subscribe<BattleInfoRequest>([this](const BattleInfoRequest &request) { handleBattleInitRequest(); });

	messageBus.subscribe<CombatActionRequest>(
	    [this](const CombatActionRequest &request) { handleCombatActionRequest(request); });
}

void BattleMode::createParties(std::vector<Engine::ECS::Entity> &partyMembers,
                               std::vector<Engine::ECS::Entity> &enemies) const
{
	StatsComponent defaultStats{.maxHP = 100,
	                            .maxMP = 100,
	                            .currHP = 100,
	                            .currMP = 100,
	                            .attack = 100,
	                            .defense = 100,
	                            .magic = 100,
	                            .resistance = 100,
	                            .skill = 100,
	                            .speed = 100};

	Prefabs::PlayerBlueprint playerBlueprint{"John Doe", {}, {}, {}, {}, {}, defaultStats, {}, {}};

	// creating player
	Prefabs::createPlayer(world, std::move(playerBlueprint));

	// creating party members
	Prefabs::PartyMemberBlueprint partyMember1{"Foo", {}, defaultStats, {}, {}, {}};
	Prefabs::PartyMemberBlueprint partyMember2{"Bar", {}, defaultStats, {}, {}, {}};
	Prefabs::PartyMemberBlueprint partyMember3{"Baz", {}, defaultStats, {}, {}, {}};

	partyMembers = {Prefabs::createPartyMember(world, std::move(partyMember1)),
	                Prefabs::createPartyMember(world, std::move(partyMember2)),
	                Prefabs::createPartyMember(world, std::move(partyMember3))};

	// creating enemies
	Prefabs::EnemyBlueprint enemy1{"Slime", {}, defaultStats, {}, {}, {}, {}};
	Prefabs::EnemyBlueprint enemy2{"Spider", {}, defaultStats, {}, {}, {}, {}};
	Prefabs::EnemyBlueprint enemy3{"Goblin", {}, defaultStats, {}, {}, {}, {}};

	enemies = {Prefabs::createEnemy(world, std::move(enemy1)), Prefabs::createEnemy(world, std::move(enemy2)),
	           Prefabs::createEnemy(world, std::move(enemy3))};
}

void BattleMode::startBattle()
{
	// start in player turn
	constexpr BattlePhaseComponent battlePhaseComponent{BattlePhase::PlayerTurn};

	// party members and enemies hardcoded
	std::vector<Engine::ECS::Entity> partyMembers;
	std::vector<Engine::ECS::Entity> enemies;
	createParties(partyMembers, enemies);

	TurnOrderComponent turnOrderComponent{.playerPartyEntities = std::move(partyMembers),
	                                      .enemyEntities = std::move(enemies)};

	RoundActionsComponent roundActionsComponent;

	Prefabs::BattleStateBlueprint blueprint{.battlePhaseComponent = battlePhaseComponent,
	                                        .turnOrderComponent = std::move(turnOrderComponent),
	                                        .roundActionsComponent = std::move(roundActionsComponent)};

	activeBattleStateEntity = Prefabs::createBattleState(world, std::move(blueprint));
}

void BattleMode::endBattle() const
{
	if (world.DestroyEntity(activeBattleStateEntity)) {
		std::cout << "Successfully destroyed battleStateEntity" << std::endl;
	} else {
		std::cout << "Error: battleStateEntity was not destroyed" << std::endl;
	}
}

// TODO: migrate request handling
/*
void BattleMode::handleBattleInitRequest() const
{
    const auto playerPartyInfo = BattleMapper::mapToCombatantInfos(getCurrentPartyMembers());
    const auto enemyPartyInfo = BattleMapper::mapToCombatantInfos(getCurrentEnemies());
    BattleInfoResponse response{.playerPartyEntities = playerPartyInfo, .enemyPartyInfo = enemyPartyInfo};
    battleModeMessageBus.publish(response);
}

void BattleMode::handleCombatActionRequest(const CombatActionRequest &combatActionRequest)
{
    // This way the phase can be accessed and changed
    auto &[currentPhase] = world.GetComponent<Engine::ECS::BattlePhaseComponent>(activeBattleStateEntity);
    currentPhase = Engine::ECS::BattlePhase::PlayerTurn;

    // map to CombatAction
    const CombatAction action = BattleMapper::mapToCombatAction(combatActionRequest);
    battleSystem.handlePlayerInputForCurrentPartyMember(action);
}

const std::vector<PartyMember> &BattleMode::getCurrentPartyMembers() const
{
    return battleSystem.getPartyMembers();
}

const std::vector<Enemy> &BattleMode::getCurrentEnemies() const
{
    return battleSystem.getEnemies();
}
*/