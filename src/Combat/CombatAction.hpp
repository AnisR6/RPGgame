#pragma once
#include <memory>
#include "Ability/Combatable.hpp"

enum ActionType {
    BASE_ATTACK,
    SPELL,
    SKILL,
    INVENTORY,
    FLIGHT
};


struct CombatAction {
    std::unique_ptr<Combatable> user;
    std::vector<std::unique_ptr<Combatable>> targets;
    ActionType actionType;

    bool operator<(const CombatAction& other) const noexcept {
        return user->getStats().speed < other.user->getStats().speed;
    }
};
