#include "BattleMode.hpp"

#include "game_state/ecs/prefabs/characters/enemy/Enemy.hpp"
#include "game_state/ecs/prefabs/characters/party_member/PartyMember.hpp"
#include "game_state/ecs/prefabs/characters/player/Player.hpp"
#include "game_state/ecs/system/combat/enemy_ai_system/EnemyAISystem.hpp"
#include "game_state/ecs/system/combat/turn_execution_system/TurnExecutionSystem.hpp"

#include <utility>

namespace Engine::ECS {
BattleMode::BattleMode(BattleMessageBus &messageBus, World &world, BattleState battleState,
                       const CombatActionExecutor executor)
    : battleModeMessageBus(messageBus), world(world), battleState(std::move(battleState)), executor(executor)
{
	// subscribe to relevant Requests
	subscribeToEvents();
	// Register Systems via SystemManager
	registerSystems();
}

void BattleMode::subscribeToEvents()
{
	battleModeMessageBus.subscribe<BattleInfoRequest>(
	    [this](const BattleInfoRequest &request) { handleBattleInitRequest(); });
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
	                            .attack = 100,
	                            .defense = 100,
	                            .magic = 100,
	                            .resistance = 100,
	                            .skill = 100,
	                            .speed = 100};

	Prefabs::PlayerBlueprint playerBlueprint{"John Doe", {}, {}, {}, {}, {}, defaultStats, {}};

	// creating party members
	Prefabs::PartyMemberBlueprint partyMember1{"Foo", {}, {}, {}};
	Prefabs::PartyMemberBlueprint partyMember2{"Bar", {}, defaultStats, {}};
	Prefabs::PartyMemberBlueprint partyMember3{"Baz", {}, defaultStats, {}};

	partyMembers = {Prefabs::createPlayer(world, std::move(playerBlueprint)),
	                Prefabs::createPartyMember(world, std::move(partyMember1)),
	                Prefabs::createPartyMember(world, std::move(partyMember2)),
	                Prefabs::createPartyMember(world, std::move(partyMember3))};

	// creating enemies
	Prefabs::EnemyBlueprint enemy1{"Slime", {}, defaultStats, {}, {}, {}};
	Prefabs::EnemyBlueprint enemy2{"Spider", {}, defaultStats, {}, {}, {}};
	Prefabs::EnemyBlueprint enemy3{"Goblin", {}, defaultStats, {}, {}, {}};

	enemies = {Prefabs::createEnemy(world, std::move(enemy1)), Prefabs::createEnemy(world, std::move(enemy2)),
	           Prefabs::createEnemy(world, std::move(enemy3))};
}

void BattleMode::startBattle()
{
	// party members and enemies hardcoded => should be received from the outside
	std::vector<Entity> partyMembers;
	std::vector<Entity> enemies;
	createParties(partyMembers, enemies);

	battleState = {
	    .battlePhaseState = {BattlePhase::PlayerTurn},
	    .turnOrderState = {.playerPartyEntities = std::move(partyMembers), .enemyEntities = std::move(enemies)},
	    .roundActionsState = {}};
}

void BattleMode::endBattle() const {}

void BattleMode::handleBattleInitRequest() const
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
	const CombatAction action = BattleMapper::mapToCombatAction(combatActionRequest);
	battleState.roundActionsState.roundActions.push_back(action);
}

} // namespace Engine::ECS