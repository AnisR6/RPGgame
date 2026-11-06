#pragma once
#include <variant>

// this event gets all spells, skills, items in inventory, etc.
// when the battle starts
struct BattleInitEvent {};

// this event contains info for the chosen action and target after a party member took its turn
struct CombatActionEvent {};

using BattleModeEvent = std::variant<BattleInitEvent, CombatActionEvent>;