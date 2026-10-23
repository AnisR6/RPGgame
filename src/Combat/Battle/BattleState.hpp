#pragma once
#include <vector>

#include "Combat/Party/PartyMember.hpp"
#include "Combat/Enemy/Enemy.hpp"


class BattleState {
	private:
		std::vector<PartyMember> partyMembers;
		std::vector<Enemy> enemies;

	public:
		bool won() const;
		bool lost() const;

};
