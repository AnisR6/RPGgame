#pragma once
#include <functional>

class BattleMenuCallbacks {

  public:
	std::function<void(std::vector<int> targetIds)> onBaseAttack;
	std::function<void(std::vector<int> targetIds, int spellId)> onSpell;
	std::function<void(std::vector<int> targetIds, int skillId)> onSkill;
	std::function<void(std::vector<int> targetIds, int itemId)> onInventory;
	std::function<void()> onFlight;
};