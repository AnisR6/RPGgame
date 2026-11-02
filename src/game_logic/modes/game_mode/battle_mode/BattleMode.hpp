#pragma once
#include "game_logic/modes/game_mode/GameMode.hpp"

enum class BattleMenuLayer {
	MAIN_COMBAT_ACTIONS,
	SPELL_SELECTION,
	SKILL_SELECTION,
	INVENTORY_SELECTION,
	TARGET_SELECTION
};

class BattleMode : public GameMode {
  private:
	BattleMenuLayer menuLayer = BattleMenuLayer::MAIN_COMBAT_ACTIONS;

  public:
	std::optional<MessageEvent> translate(const sf::Event &event) override;
};