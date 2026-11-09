#pragma once

#include "BattleState.hpp"
#include "Structure/CombatAction.hpp"
#include "Structure/CombatSkill.hpp"
#include "Structure/Spell.hpp"
#include "game_state/ecs/capabilities/Combatable.hpp"

class BattleSystem {
  private:
	BattleState battleState;
	int currentPartyMemberIndex = 0;
	std::vector<CombatAction> roundActions;

	void getEnemyActions();
	void executeRound();

  public:
	[[nodiscard]] const std::vector<PartyMember> &getPartyMembers() const;
	[[nodiscard]] const std::vector<Enemy> &getEnemies() const;
	void handlePlayerInputForCurrentPartyMember(const CombatAction &action);
};