#include "BattleMode.hpp"

BattleMode::BattleMode(BattleMessageBus &messageBus) : battleModeMessageBus(messageBus)
{
	messageBus.subscribe<BattleInfoRequest>([this](const BattleInfoRequest &request) { handleBattleInitRequest(); });

	messageBus.subscribe<CombatActionRequest>(
	    [this](const CombatActionRequest &request) { handleCombatActionRequest(request); });
}

void BattleMode::handleBattleInitRequest() const
{
	const auto playerPartyInfo = BattleMapper::mapToCombatantInfos(getCurrentPartyMembers());
	const auto enemyPartyInfo = BattleMapper::mapToCombatantInfos(getCurrentEnemies());
	BattleInfoResponse response{.playerPartyInfo = playerPartyInfo, .enemyPartyInfo = enemyPartyInfo};
	battleModeMessageBus.publish(response);
}

void BattleMode::handleCombatActionRequest(const CombatActionRequest &combatActionRequest)
{
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
