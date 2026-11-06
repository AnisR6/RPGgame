#include "BattleMode.hpp"

void BattleMode::handleEvent(BattleModeEvent &battleModeEvent) const
{
	std::visit(
	    [this](auto &&event) {
		    using T = std::decay_t<decltype(battleModeEvent)>;

		    if constexpr (std::is_same_v<T, BattleInitEvent>) {

		    } else if constexpr (std::is_same_v<T, CombatActionEvent>) {
		    }
	    },
	    battleModeEvent);
}