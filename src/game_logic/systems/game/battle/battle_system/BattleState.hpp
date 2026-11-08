#pragma once
#include <vector>

#include "game_logic/systems/game/battle/battle_system/Enemy/Enemy.hpp"
#include "game_logic/systems/game/battle/battle_system/Party/PartyMember.hpp"

class BattleState {
  private:
	std::vector<PartyMember> partyMembers;
	std::vector<Enemy> enemies;

  public:
	[[nodiscard]] const std::vector<PartyMember> &getPartyMembers() const;
	std::vector<PartyMember> &getPartyMembers();
	[[nodiscard]] const std::vector<Enemy> &getEnemies() const;
	std::vector<Enemy> &getEnemies();
	[[nodiscard]] bool won() const;
	[[nodiscard]] bool lost() const;
};
