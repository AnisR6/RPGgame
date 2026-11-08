#pragma once
#include "application/battle/CombatantInfo.hpp"
#include "application/messages/BattleMessage.hpp"
#include "game_logic/systems/game/battle/battle_system/Enemy/Enemy.hpp"
#include "game_logic/systems/game/battle/battle_system/Party/PartyMember.hpp"

class BattleMapper {
  public:
	static CombatAction mapToCombatAction(const CombatActionRequest &request);
	static CombatantInfo mapToCombatantInfo(const PartyMember &partyMember);
	static CombatantInfo mapToCombatantInfo(const Enemy &enemy);
	template <typename T>
	static std::vector<CombatantInfo> mapToCombatantInfos(const std::vector<T> &combatants);
};