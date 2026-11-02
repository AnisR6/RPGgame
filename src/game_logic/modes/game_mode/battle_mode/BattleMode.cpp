#include "BattleMode.hpp"

std::optional<MessageEvent> BattleMode::translate(const sf::Event &event)
{
	if (event.is<sf::Event::KeyPressed>()) {
		if (menuLayer == BattleMenuLayer::MAIN_COMBAT_ACTIONS) {

		} else if (menuLayer == BattleMenuLayer::SPELL_SELECTION) {

		} else if (menuLayer == BattleMenuLayer::SKILL_SELECTION) {

		} else if (menuLayer == BattleMenuLayer::INVENTORY_SELECTION) {

		} else if (menuLayer == BattleMenuLayer::TARGET_SELECTION) {
		}
	}
	return std::nullopt;
}