#include "BattleMode.hpp"

BattleMode::BattleMode(BattleMessageBus &eventBus) : battleModeEventBus(eventBus)
{
	eventBus.subscribe<BattleInfoRequest>(
	    [this](const BattleInfoRequest &request) { handleBattleInitRequest(request); });

	eventBus.subscribe<CombatActionRequest>(
	    [this](const CombatActionRequest &request) { handleCombatActionRequest(request); });
}

void BattleMode::handleBattleInitRequest(const BattleInfoRequest &battleInfoRequest) {}

void BattleMode::handleCombatActionRequest(const CombatActionRequest &combatActionEvent) {}

const std::vector<PartyMember> &BattleMode::getCurrentPartyMembers() const
{
	return battleSystem.getPartyMembers();
}

const std::vector<Enemy> &BattleMode::getCurrentEnemies() const
{
	return battleSystem.getEnemies();
}
