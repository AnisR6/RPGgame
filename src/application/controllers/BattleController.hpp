#pragma once

#include "application/bus/BattleModeEventBus.hpp"
#include "game_logic/modes/game_mode/battle_mode/BattleMode.hpp"

#include <memory>

class BattleController {
  private:
	BattleModeEventBus battleModeEventBus;
	std::unique_ptr<BattleMode> battleMode;

  public:
	void update(float dt);
};