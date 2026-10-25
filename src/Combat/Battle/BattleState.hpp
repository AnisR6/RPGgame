#pragma once
#include <vector>

#include "Combat/Party/PartyMember.hpp"
#include "Combat/Enemy/Enemy.hpp"


class BattleState {
	private:
		std::vector<PartyMember> partyMembers;
		std::vector<Enemy> enemies;

	public:

		const std::vector<PartyMember>& getPartyMembers() const;
		std::vector<PartyMember>& getPartyMembers();
		const std::vector<Enemy>& getEnemies() const;
		std::vector<Enemy>& getEnemies();
		bool won() const;
		bool lost() const;

};
