#pragma once
#include "presentation/screens/core/Screen.hpp"

class BattleScreen : public Screen {
  private:
	BattleScreen();

  public:
	static BattleScreen &getInstance();

	BattleScreen(const BattleScreen &) = delete;
	BattleScreen &operator=(const BattleScreen &) = delete;
};