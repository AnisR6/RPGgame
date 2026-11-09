#include "BattleSystem.hpp"
#include <algorithm>

void BattleSystem::handlePlayerInputForCurrentPartyMember(const CombatAction &action)
{
	const auto &party = battleState.getPartyMembers();
	roundActions.push_back(action);

	currentPartyMemberIndex++;

	if (currentPartyMemberIndex >= party.size()) {
		currentPartyMemberIndex = 0;
		executeRound();
	}
}

void BattleSystem::getEnemyActions()
{
	for (auto &enemy : battleState.getEnemies()) {
		// consult enemy ai to choose an action
	}
}

void BattleSystem::executeRound()
{
	getEnemyActions();

	// sort actions by speed
	std::ranges::sort(roundActions, std::greater<CombatAction>{});

	for (auto &action : roundActions) {
		if (action.actor != nullptr && action.actor->isAlive()) {
			action.evalCombatAction();
		}

		if (battleState.won() || battleState.lost()) {
			break;
		}
	}
	roundActions.clear();
}

const std::vector<PartyMember> &BattleSystem::getPartyMembers() const
{
	return battleState.getPartyMembers();
}

const std::vector<Enemy> &BattleSystem::getEnemies() const
{
	return battleState.getEnemies();
}