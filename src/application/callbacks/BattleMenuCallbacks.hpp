#pragma once
#include <functional>

class BattleMenuCallbacks {

  public:
	std::function<void()> onBaseAttack;
	std::function<void(int)> onSpell;
	std::function<void(int)> onSkill;
	std::function<void(int)> onInventory;
	std::function<void()> onFlight;
};