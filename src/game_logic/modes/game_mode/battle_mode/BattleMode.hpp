#pragma once
#include "application/events/BattleModeEvent.hpp"

class BattleMode {
  public:
	void handleEvent(BattleModeEvent &battleModeEvent) const;
};