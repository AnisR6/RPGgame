#pragma once
#include "application/combat/menu_entries/BattleActionMenuEntry.hpp"

#include <optional>
#include <vector>

struct BattleActorMenuData {
        std::optional<BattleActionMenuEntry> baseAttack;
        std::optional<BattleActionMenuEntry> flee;
        std::vector<BattleActionMenuEntry> spells;
        std::vector<BattleActionMenuEntry> skills;
};
