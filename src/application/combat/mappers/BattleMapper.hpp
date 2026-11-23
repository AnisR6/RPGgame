#pragma once

#include "application/combat/messages/BattleMessage.hpp"
#include "ecs/combat/systems/CombatAction.hpp"

class BattleMapper {
    public:
        static CombatAction MapToCombatAction(const CombatActionRequest& request);
};
