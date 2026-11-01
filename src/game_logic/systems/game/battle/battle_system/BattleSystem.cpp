#include "BattleSystem.hpp"
#include <algorithm>

void BattleSystem::getPartyMemberActions(std::vector<CombatAction> &roundActions)
{
	for (auto &partyMember : battleState.getPartyMembers()) {
		// consult some interface that gets player actions through gui input
	}
}

void BattleSystem::getEnemyActions(std::vector<CombatAction> &roundActions)
{
	for (auto &enemy : battleState.getEnemies()) {
		// consult enemy ai to choose an action
	}
}

void BattleSystem::executeRound()
{
	std::vector<CombatAction> roundActions;
	getPartyMemberActions(roundActions);
	getEnemyActions(roundActions);

	// sort actions by speed
	std::ranges::sort(roundActions, std::greater<CombatAction>{});

	for (auto &action : roundActions) {
		if (action.actor != nullptr && action.actor->isAlive()) {
			action.evalCombatAction();
		}
	}
}