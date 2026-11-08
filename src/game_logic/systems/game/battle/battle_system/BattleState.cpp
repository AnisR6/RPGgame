#include "BattleState.hpp"

bool BattleState::lost() const
{
	for (auto &member : partyMembers) {
		if (member.isAlive()) {
			return false;
		}
	}
	return true;
}

bool BattleState::won() const
{
	for (auto &enemy : enemies) {
		if (enemy.isAlive()) {
			return false;
		}
	}
	return true;
}

const std::vector<PartyMember> &BattleState::getPartyMembers() const
{
	return partyMembers;
}

std::vector<PartyMember> &BattleState::getPartyMembers()
{
	return partyMembers;
}
const std::vector<Enemy> &BattleState::getEnemies() const
{
	return enemies;
}

std::vector<Enemy> &BattleState::getEnemies()
{
	return enemies;
}
