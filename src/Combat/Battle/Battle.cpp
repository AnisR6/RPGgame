#include "Battle.hpp"

void Battle::getPartyMemberActions(std::vector<CombatAction> &roundActions)
{
	for (auto &partyMember : battleState.getPartyMembers()) {
		// consult some interface that gets player actions through gui input
	}
}

void Battle::getEnemyActions(std::vector<CombatAction> &roundActions)
{
	for (auto &enemy : battleState.getEnemies()) {
		// consult enemy ai to choose an action
	}
}

void Battle::runRound()
{
	std::vector<CombatAction> roundActions;
	getPartyMemberActions(roundActions);
	getEnemyActions(roundActions);

	// sort actions by speed
	std::sort(roundActions.begin(), roundActions.end());

	for (auto &action : roundActions) {
		// evalCombatAction(action);
	}
}