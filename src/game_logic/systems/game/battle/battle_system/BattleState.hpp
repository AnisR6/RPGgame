#pragma once
#include <vector>

#include "game_logic/systems/game/battle/battle_system/Enemy/Enemy.hpp"
#include "game_logic/systems/game/battle/battle_system/Party/PartyMember.hpp"

class BattleState {
  private:
	std::vector<PartyMember> partyMembers;
	std::vector<Enemy> enemies;

  public:
	const std::vector<PartyMember> &getPartyMembers() const;
	std::vector<PartyMember> &getPartyMembers();
	const std::vector<Enemy> &getEnemies() const;
	std::vector<Enemy> &getEnemies();
	bool won() const;
	bool lost() const;
};
