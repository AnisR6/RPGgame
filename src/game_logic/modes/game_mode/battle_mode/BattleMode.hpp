#pragma once
#include "application/bus/BattleMessageBus.hpp"
#include "game_logic/mappers/BattleMapper.hpp"
#include "game_logic/systems/game/battle/battle_system/BattleSystem.hpp"

class BattleMode {
  private:
	BattleSystem battleSystem;
	BattleMapper battleMapper;
	BattleMessageBus &battleModeMessageBus;

	[[nodiscard]] const std::vector<PartyMember> &getCurrentPartyMembers() const;
	[[nodiscard]] const std::vector<Enemy> &getCurrentEnemies() const;
	void handleBattleInitRequest() const;
	void handleCombatActionRequest(const CombatActionRequest &combatActionRequest);

  public:
	explicit BattleMode(BattleMessageBus &messageBus);
};